// Fill out your copyright notice in the Description page of Project Settings.


#include "LDCharacterBase.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/LDAbilitySet.h"
#include "AbilitySystem/LDAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"

// Sets default values
ALDCharacterBase::ALDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ViewCursorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerCursorCollision"));
	ViewCursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerCursorMesh"));
}

#pragma region ~AActor Interface

void ALDCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//绑定技能输入
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
	&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);
	
	//子类这里处理自己的Native Input
}

void ALDCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!Cast<ALDPlayerController>(NewController))
	{
		return;
	}

	ViewCursorCollision->SetVisibility(true);
	ViewCursorMesh->SetVisibility(true);
	ViewCursorCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	// AbilitySystemComponent
	ALDPlayerState* PS = Cast<ALDPlayerState>(GetPlayerState());
	if (IsValid(PS))
	{
		AbilitySystemComponent = Cast<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS,this);

		if(AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
		}
	}
}

void ALDCharacterBase::UnPossessed()
{
	Super::UnPossessed();

	ViewCursorCollision->SetVisibility(false);
	ViewCursorMesh->SetVisibility(false);
	ViewCursorCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ALDCharacterBase::BeginPlay()
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
}

void ALDCharacterBase::OnRep_PlayerState()
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

void ALDCharacterBase::OnZoomValueChanged()
{
}

void ALDCharacterBase::UpdateCursorPosition(FVector OffsetDirection)
{
}


#pragma endregion

#pragma region GAS Input

void ALDCharacterBase::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ALDCharacterBase::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

#pragma endregion

#pragma region Character View

void ALDCharacterBase::SetCurrentCursorPosition(const FVector& TargetPos) const
{
	ViewCursorCollision->SetWorldLocation(TargetPos, false);

	//UpdateCursorPosition
	FTransform CursorTargetTransform;
	//TODO:后续这里调整白圈大小
	CursorTargetTransform = FTransform(ViewCursorCollision->GetComponentRotation(),
	                                   ViewCursorCollision->GetComponentLocation(), FVector(2, 2, 1));
	if (ViewCursorMesh)
	{
		UWorld* World = GetWorld();
		float DeltaTime = World ? World->GetDeltaSeconds() : 0.0f;
		FTransform CursorNewTransform = UKismetMathLibrary::TInterpTo(ViewCursorMesh->GetComponentTransform(),
		                                                              CursorTargetTransform, DeltaTime, 12.0);
		ViewCursorMesh->SetWorldTransform(CursorNewTransform);
	}
}

void ALDCharacterBase::DragCharacterPosition(const FVector& TargetHandle)
{
	//拖动移动
	FVector CurrentPos;
	bool bIsMousePos = false;
	// MouseToGroundPlane(CurrentPos, bIsMousePos);
	ALDPlayerController* PC = GetController<ALDPlayerController>();
	if (!IsValid(PC))
	{
		return;
	}

	PC->MouseToGroundPlane(CurrentPos,bIsMousePos);
	
	FVector CameraBoomLocation = CameraBoom->GetComponentLocation();
	FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
	FVector CameraBoomForward = CameraBoom->GetForwardVector();
	
	CameraBoomForward = CameraBoomForward * (CameraBoom->TargetArmLength - CameraBoom->
		SocketOffset.X);
	CameraBoomForward = CameraBoomForward * -1.0f;
	FVector CameraBoomUp = CameraBoom->GetUpVector() * CameraBoom->SocketOffset.Z;
	FVector IntersectionVector = (CameraBoomForward + CameraBoomUp + CameraBoomLocation) - CameraLocation;
	FVector StoredMove = TargetHandle - CurrentPos - IntersectionVector;

	UpdateCursorPosition(FVector(StoredMove.X, StoredMove.Y, 0));
}

void ALDCharacterBase::UpdateCharacterViewZoom(float ZoomDirection)
{
	if (!IsValid(ZoomCurve))
	{
		UE_LOG(LogLDCharacter, Error, TEXT("Set ZoomCurve In BP!"))
		return;
	}
	
	//距离
	ZoomValue = FMath::Clamp(ZoomDirection * 0.01 + ZoomValue,0.0,1.0);
	CameraBoom->TargetArmLength = FMath::Lerp(800, 40000, ZoomCurve->GetFloatValue(ZoomValue));
	
	//镜头微量偏转
	float RotationYValue = FMath::Lerp(-40, -55, ZoomCurve->GetFloatValue(ZoomValue));
	FRotator NewRotation = FRotator(RotationYValue, 0.0f, 0.0f);
	CameraBoom->SetRelativeRotation(NewRotation);

	OnZoomValueChanged();
	
	//TODO:镜头后处理
	
	//镜头可视角度
	TopDownCameraComponent->SetFieldOfView(	FMath::Lerp(20, 15, ZoomCurve->GetFloatValue(ZoomValue)));
}

#pragma endregion
