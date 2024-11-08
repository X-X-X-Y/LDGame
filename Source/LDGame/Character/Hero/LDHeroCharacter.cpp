// Fill out your copyright notice in the Description page of Project Settings.


#include "LDHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/LDPlayerController.h"


ALDHeroCharacter::ALDHeroCharacter()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

#pragma region UR Behaviour
void ALDHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(ALDPlayerController* PC = Cast<ALDPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UpdatePlayerViewZoom();
}
#pragma endregion

#pragma region PlayerInput

void ALDHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//绑定技能输入
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
	&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

	//绑定玩家视角输入
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Move, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
	// LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Spin, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerSpin);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerZoom);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerSelect);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_DMove, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerDragMove);
}

void ALDHeroCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void ALDHeroCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
}
#pragma region Player Native Input

void ALDHeroCharacter::OnPlayerMove(const FInputActionValue& InputValue)
{
	ALDPlayerController* PC = Cast<ALDPlayerController>(GetController());
	if (PC)
	{
		const FVector2D Value = InputValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ALDHeroCharacter::OnPlayerSelect(const FInputActionValue& InputValue)
{
	//这里处理点击逻辑
	//1-检查交互点(如果是可交互的走对应的交互逻辑)
	//	1-获取鼠标位置并映射到世界空间中
	//  2-镜头指向当前点 和 镜头指向交点的向量 做向量减法,得到移动方向
	//  3-鼠标拖拽的值运用到移动方向的向量上
}

void ALDHeroCharacter::OnPlayerSpin(const FInputActionValue& Value)
{
}

void ALDHeroCharacter::OnPlayerZoom(const FInputActionValue& InputValue)
{
	if (ZoomCurve != nullptr)
	{
		ZoomDirection = InputValue.Get<float>();
		UpdatePlayerViewZoom();
	}
}

void ALDHeroCharacter::OnPlayerDragMove(const FInputActionValue& InputValue)
{
	ALDPlayerController* PC = Cast<ALDPlayerController>(GetController());
	if (PC)
	{
		//1-获取相机到选中点的向量 2-鼠标和
		
		// FVector2d CurrentMousePos;
		// FVector WorldPosition;
		// FVector WorldDirection;
		// float TempValue = 0;
		// FVector Intersection;
		// PC->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);
		// if (PC->DeprojectScreenPositionToWorld(CurrentMousePos.X, CurrentMousePos.Y, WorldPosition, WorldDirection))
		// {
		// 	FPlane CurrentPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector(0,0,0),FVector(0,0,0));
		// 	WorldDirection = WorldDirection * 100000 + WorldPosition;
		//
		// 	UKismetMathLibrary::LinePlaneIntersection(WorldPosition, WorldDirection, CurrentPlane, TempValue, Intersection);
		// }
	}
}

#pragma endregion

#pragma endregion

#pragma region PlayerCamerView

void ALDHeroCharacter::UpdatePlayerViewZoom()
{
	//距离
	ZoomValue = FMath::Clamp(ZoomDirection * 0.01 + ZoomValue,0.0,1.0);
	CameraBoom->TargetArmLength = FMath::Lerp(800, 40000, ZoomCurve->GetFloatValue(ZoomValue));

	//镜头微量偏转
	float RotationYValue = FMath::Lerp(-40, -55, ZoomCurve->GetFloatValue(ZoomValue));
	FRotator NewRotation = FRotator(RotationYValue, 0.0f, 0.0f);
	CameraBoom->SetRelativeRotation(NewRotation);

	//镜头越远移动速度越快
	UCharacterMovementComponent* HeroMoveComp = GetCharacterMovement();
	if (HeroMoveComp)
	{
		HeroMoveComp->MaxWalkSpeed = FMath::Lerp(1000, 6000, ZoomCurve->GetFloatValue(ZoomValue));
	}

	//TODO:镜头后处理

	//镜头可视角度
	TopDownCameraComponent->SetFieldOfView(	FMath::Lerp(20, 15, ZoomCurve->GetFloatValue(ZoomValue)));
}

#pragma endregion
