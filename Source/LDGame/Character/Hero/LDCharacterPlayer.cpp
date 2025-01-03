// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "LevelActor/LevelInteractable/LDInteractableBuildPlane.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"

#pragma region CharacterInterface

ALDCharacterPlayer::ALDCharacterPlayer()
{
	PlayerPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PlayerPawnMovement"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	ViewCursorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ViewCursorCollision"));
	ViewCursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ViewCursorMesh"));
}

void ALDCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Move, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerZoom);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Started, this, &ThisClass::OnPlayerSelect);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Completed, this, &ThisClass::OnPlayerSelectComplete);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_DMove, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerDragMove);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_HSelect, ETriggerEvent::Started, this, &ThisClass::OnHeroSelect);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_BuildMode, ETriggerEvent::Started, this, &ThisClass::OnBuildModeChange);
}

void ALDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CurrentSelectState = EPlayerSelectState::OnSelectNone;
}

void ALDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->Deactivate();
		GetCharacterMovement()->PrimaryComponentTick.bCanEverTick = false;
	}

	//ViewZoom
	UpdatePlayerViewZoom();

	//PlayerViewPos
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(MoveTrackingTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::MoveTracking),0.02f,true);
	}

	ViewCursorCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerEntryBuildPlane);
	ViewCursorCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPlayerExitBuildPlane);
}

#pragma endregion

#pragma region Native Input

void ALDCharacterPlayer::OnPlayerMove(const FInputActionValue& InputValue)
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

void ALDCharacterPlayer::OnPlayerSelect(const FInputActionValue& InputValue)
{
	//鼠标点击后获取在平面上的坐标
	MouseToGroundPlane(TargetHandle, bIsMousePos);
	bIsLeftMouseButtonDown = true;
}

void ALDCharacterPlayer::OnPlayerSelectComplete(const FInputActionValue& InputValue)
{
	bIsLeftMouseButtonDown = false;
}

void ALDCharacterPlayer::OnPlayerSpin(const FInputActionValue& Value)
{
}

void ALDCharacterPlayer::OnPlayerZoom(const FInputActionValue& InputValue)
{
	if (ZoomCurve != nullptr)
	{
		ZoomDirection = InputValue.Get<float>();
		UpdatePlayerViewZoom();
	}
}

void ALDCharacterPlayer::OnPlayerDragMove(const FInputActionValue& InputValue)
{
	if (!bIsLeftMouseButtonDown)
	{
		return;
	}
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

void ALDCharacterPlayer::OnPlayerHeroSelect(const FInputActionValue& InputValue)
{
	//TODO
}

void ALDCharacterPlayer::OnHeroSelect(const FInputActionValue& InputValue)
{
}

void ALDCharacterPlayer::OnBuildModeChange(const FInputActionValue& InputValue)
{
	ALDPlayerState* PS = GetPlayerState<ALDPlayerState>();
	if (IsValid(PS))
	{
		switch (PS->GetPlayerSelectState())
		{
		case EPlayerSelectState::OnSelectNone:
			PS->SetPlayerSelectState(EPlayerSelectState::OnSelectBuilding);
			break;
		case EPlayerSelectState::OnSelectBuilding:
			PS->SetPlayerSelectState(EPlayerSelectState::OnSelectNone);
			break;
		default: ;
		}
	}
}

#pragma endregion

#pragma region Player View

void ALDCharacterPlayer::MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const
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

void ALDCharacterPlayer::MoveTracking()
{
	FVector CurrentTargetPos;
	MouseToGroundPlane(CurrentTargetPos, bIsMousePos);

	if (!IsValid(ViewCursorCollision) || !ViewCursorCollision)
	{
		UE_LOG(LogLDCharacter, Error, TEXT("CapsuleComp is nullptr!"));
		return;
	}
	
	
	CurrentTargetPos += FVector(0, 0, 10);

	if (ALDPlayerState* PS = GetPlayerState<ALDPlayerState>(); IsValid(PS))
	{
		if (PS->GetPlayerSelectState() == EPlayerSelectState::OnSelectBuilding)
		{
			if (IsValid(BuildPlane.Get()))
			{
				CurrentTargetPos = BuildPlane->GetBuildPlanPointPosition(CurrentTargetPos);
				//UE_LOG(LogLD, Log, TEXT("TargetPos X=%f, Y=%f, Z=%f"), CurrentTargetPos.X, CurrentTargetPos.Y, CurrentTargetPos.Z);
			}
		}
	}
	
	ViewCursorCollision->SetWorldLocation(CurrentTargetPos,false);

	//UpdateCursorPosition
	FTransform CursorTargetTransform;
	//TODO:后续这里调整白圈大小
	CursorTargetTransform = FTransform(ViewCursorCollision->GetComponentRotation(),
	                                   ViewCursorCollision->GetComponentLocation(), FVector(2, 2, 1));
	if (ViewCursorMesh)
	{
		UWorld* World = GetWorld();
		float DeltaTime = World? World->GetDeltaSeconds() : 0.0f;
		FTransform CursorNewTransform = UKismetMathLibrary::TInterpTo(ViewCursorMesh->GetComponentTransform(),CursorTargetTransform,DeltaTime,12.0);
		ViewCursorMesh->SetWorldTransform(CursorNewTransform);
		// UE_LOG(LogLD, Log, TEXT("CursorTargetTransform X=%f, Y=%f, Z=%f"), CursorTargetTransform.GetLocation().X, CursorTargetTransform.GetLocation().Y, CursorTargetTransform.GetLocation().Z);
	}
}

void ALDCharacterPlayer::UpdatePlayerViewZoom()
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


#pragma endregion

#pragma region BuildPlane

void ALDCharacterPlayer::OnPlayerEntryBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (Cast<ALDInteractableBuildPlane>(OtherActor))
	{
		BuildPlane = Cast<ALDInteractableBuildPlane>(OtherActor);
	}
}

void ALDCharacterPlayer::OnPlayerExitBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BuildPlane.Reset();
	BuildPlane = nullptr;
}

ALDInteractableBuildPlane* ALDCharacterPlayer::GetInteractableBuildPlane() const
{
	return BuildPlane.Get();
}

#pragma endregion
