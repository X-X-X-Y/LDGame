// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerPawn.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Character/AbilitySystem//LDAbilitySet.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"


ALDPlayerPawn::ALDPlayerPawn()
{
	PlayerPawnRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPawnRoot"));
	SetRootComponent(PlayerPawnRoot);
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PlayerPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PlayerPawnMovement"));
	PlayerCursorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerCursorCollision"));
	PlayerCursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerCursorMesh"));

}

#pragma region UR Behaviour

void ALDPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// AbilitySystemComponent
	ALDPlayerState* PS = Cast<ALDPlayerState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(PS,this);

	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
	}
}

void ALDPlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ALDPlayerState* PS = Cast<ALDPlayerState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);

	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
	}
}

void ALDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//Input
	if(ALDPlayerController* PC = Cast<ALDPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//ViewZoom
	UpdatePlayerViewZoom();

	//PlayerViewPos
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(MoveTrackingTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::MoveTracking),0.02f,true);
	}
}

#pragma endregion

#pragma region PlayerInput

void ALDPlayerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
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
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Started, this, &ThisClass::OnPlayerSelect);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_DMove, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerDragMove);
}

//这里绑定技能输入的InputTag
void ALDPlayerPawn::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ALDPlayerPawn::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}
#pragma region Player Native Input

void ALDPlayerPawn::OnPlayerMove(const FInputActionValue& InputValue)
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

void ALDPlayerPawn::OnPlayerSelect(const FInputActionValue& InputValue)
{
	//鼠标点击后获取在平面上的坐标
	MouseToGroundPlane(TargetHandle, bIsMousePos);
}

void ALDPlayerPawn::OnPlayerSpin(const FInputActionValue& Value)
{
}

void ALDPlayerPawn::OnPlayerZoom(const FInputActionValue& InputValue)
{
	if (ZoomCurve != nullptr)
	{
		ZoomDirection = InputValue.Get<float>();
		UpdatePlayerViewZoom();
	}
}

void ALDPlayerPawn::OnPlayerDragMove(const FInputActionValue& InputValue)
{
	//拖动移动
	FVector CurrentPos;
	MouseToGroundPlane(CurrentPos, bIsMousePos);
	
	FVector CameraBoomLocation = CameraBoom->GetComponentLocation();
	FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
	FVector CameraBoomForward = CameraBoom->GetForwardVector();
	
	CameraBoomForward = CameraBoomForward * (CameraBoom->TargetArmLength - CameraBoom->
		SocketOffset.X);
	CameraBoomForward = CameraBoomForward * -1.0f;
	FVector CameraBoomUp = CameraBoom->GetUpVector() * CameraBoom->SocketOffset.Z;
	FVector IntersectionVector = (CameraBoomForward + CameraBoomUp + CameraBoomLocation) - CameraLocation;
	FVector StoredMove = TargetHandle - CurrentPos - IntersectionVector;

	AddActorWorldOffset(FVector(StoredMove.X, StoredMove.Y, 0));
}

#pragma endregion

#pragma endregion

#pragma region PlayerView Helper

void ALDPlayerPawn::UpdatePlayerViewZoom()
{
	//距离
	ZoomValue = FMath::Clamp(ZoomDirection * 0.01 + ZoomValue,0.0,1.0);
	CameraBoom->TargetArmLength = FMath::Lerp(800, 40000, ZoomCurve->GetFloatValue(ZoomValue));

	//镜头微量偏转
	float RotationYValue = FMath::Lerp(-40, -55, ZoomCurve->GetFloatValue(ZoomValue));
	FRotator NewRotation = FRotator(RotationYValue, 0.0f, 0.0f);
	CameraBoom->SetRelativeRotation(NewRotation);

	//镜头越远移动速度越快
	if (PlayerPawnMovement)
	{
		PlayerPawnMovement->MaxSpeed = FMath::Lerp(1000, 6000, ZoomCurve->GetFloatValue(ZoomValue));
	}

	//TODO:镜头后处理

	//镜头可视角度
	TopDownCameraComponent->SetFieldOfView(	FMath::Lerp(20, 15, ZoomCurve->GetFloatValue(ZoomValue)));
}

void ALDPlayerPawn::MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const
{
	//1-获取相机到选中点的向量 2-鼠标和
		
	FVector2d CurrentPos;
	FVector WorldPosition;
	FVector WorldDirection;
	float TempValue = 0;

	
	ALDPlayerController* PC = Cast<ALDPlayerController>(GetController());
	if (PC == nullptr)
	{
		bIsMouse = false;
		return;
	}

	if (PC->GetMousePosition(CurrentPos.X, CurrentPos.Y))
	{
		//鼠标位置能够获取
		PC->DeprojectScreenPositionToWorld(CurrentPos.X, CurrentPos.Y, WorldPosition, WorldDirection);
		bIsMouse = true;
	}
	else
	{
		int32 SizeX, SizeY;
		PC->GetViewportSize(SizeX, SizeY);
		FVector2D ViewportSize = FVector2D(SizeX, SizeY);
		CurrentPos = UKismetMathLibrary::Divide_Vector2DVector2D(ViewportSize,FVector2d(2.0,2.0));
		PC->DeprojectScreenPositionToWorld(CurrentPos.X, CurrentPos.Y, WorldPosition, WorldDirection);
		bIsMouse = false;
	}

	FPlane CurrentPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector(0, 0, 0), FVector(0, 0, 1));
	FVector LineEnd = WorldPosition + (WorldDirection * 100000);

	UKismetMathLibrary::LinePlaneIntersection(WorldPosition, LineEnd, CurrentPlane, TempValue, Intersection);
}

void ALDPlayerPawn::MoveTracking()
{
	FVector CurrentTargetPos;
	MouseToGroundPlane(CurrentTargetPos, bIsMousePos);

	if (!IsValid(PlayerCursorCollision) || !PlayerCursorCollision)
	{
		UE_LOG(LogLDCharacter, Error, TEXT("CapsuleComp is nullptr!"));
		return;
	}
	CurrentTargetPos += FVector(0, 0, 10);
	PlayerCursorCollision->SetWorldLocation(CurrentTargetPos,false);
	
	//UpdateCursorPosition
	FTransform CursorTargetTransform;
	//TODO:后续这里调整白圈大小
	CursorTargetTransform = FTransform(PlayerCursorCollision->GetComponentRotation(),PlayerCursorCollision->GetComponentLocation(),FVector(2,2,1));
	if (PlayerCursorMesh)
	{
		UWorld* World = GetWorld();
		float DeltaTime = World? World->GetDeltaSeconds() : 0.0f;
		FTransform CursorNewTransform = UKismetMathLibrary::TInterpTo(PlayerCursorMesh->GetComponentTransform(),CursorTargetTransform,DeltaTime,12.0);
		PlayerCursorMesh->SetWorldTransform(CursorNewTransform);
	}
}

#pragma endregion
