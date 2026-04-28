// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerController.h"
#include "DWInteractable.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "NavigationSystem.h"
#include "Engine/Engine.h"

ADWPlayerController::ADWPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ADWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, MappingPriority);
			}
		}
	}

	SetCharacterMoveSpeed(WalkSpeed);
}

void ADWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SetDestinationAction)
		{
			EnhancedInput->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &ADWPlayerController::HandleDestinationStarted);
			EnhancedInput->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &ADWPlayerController::HandleDestinationCompleted);
			EnhancedInput->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &ADWPlayerController::HandleDestinationCompleted);
		}
	}
}

void ADWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsDestinationHeld || !GetWorld())
	{
		return;
	}

	const float HeldTime = GetWorld()->GetTimeSeconds() - DestinationHoldStartTime;

	if (!bIsActionHoldMode && HeldTime >= HoldToActionThreshold)
	{
		EnterActionHoldMode();
	}

	if (bIsActionHoldMode)
	{
		MoveDirectlyTowardCursor();
	}
}

void ADWPlayerController::HandleDestinationStarted()
{
	bIsDestinationHeld = true;
	bIsActionHoldMode = false;

	if (GetWorld())
	{
		DestinationHoldStartTime = GetWorld()->GetTimeSeconds();
	}

	IssueCommandUnderCursor(true);
}

void ADWPlayerController::HandleDestinationCompleted()
{
	bIsDestinationHeld = false;

	GetWorldTimerManager().ClearTimer(FollowCursorTimerHandle);

	if (bIsActionHoldMode)
	{
		ExitActionHoldMode();
	}
}

void ADWPlayerController::UpdateHeldDestination()
{
	/*if (!bIsDestinationHeld || !GetWorld())
	{
		return;
	}
	const float HeldTime = GetWorld()->GetTimeSeconds() - DestinationHoldStartTime;

	if (!bIsActionHoldMode && HeldTime >= HoldToActionThreshold)
	{
		EnterActionHoldMode();
	}

	if (bIsActionHoldMode)
	{
		MoveDirectlyTowardCursor();
	}*/
}

void ADWPlayerController::EnterActionHoldMode()
{
	bIsActionHoldMode = true;

	ClearPendingInteract();

	StopMovement();

	SetIgnoreMoveInput(false);

	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());

	if (ControlledCharacter)
	{
		UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement();

		if (MoveComp)
		{
			MoveComp->SetMovementMode(MOVE_Walking);

			MoveComp->MaxWalkSpeed = RunSpeed;
		}
	}
}

void ADWPlayerController::ExitActionHoldMode()
{
	bIsActionHoldMode = false;

	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());

	if (ControlledCharacter)
	{
		UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement();

		if (MoveComp)
		{
			MoveComp->SetMovementMode(MOVE_Walking);
			MoveComp->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void ADWPlayerController::MoveDirectlyTowardCursor()
{
	APawn* ControlledPawn = GetPawn();

	if (!ControlledPawn || !GetWorld())
	{
		return;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;

	if (!GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	FVector WorldOrigin;
	FVector WorldDirection;

	if (!DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDirection)) {
		return;
	}

	const FVector TraceStart = WorldOrigin;
	const FVector TraceEnd = TraceStart + WorldDirection * 100000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn);
	QueryParams.bTraceComplex = true;

	FHitResult Hit;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	if (!bHit)
	{
		return;
	}

	FVector Direction = Hit.ImpactPoint - ControlledPawn->GetActorLocation();
	Direction.Z = 0.0f;

	if (Direction.SizeSquared() < FMath::Square(20.0f))
	{
		return;
	}

	ControlledPawn->AddMovementInput(Direction.GetSafeNormal(), 1.0f, true);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1001,
			0.05f,
			FColor::Green,
			FString::Printf(TEXT("Action Hold Move | Dist: %.1f | Hit: %s"),
				Direction.Size(),
				*GetNameSafe(Hit.GetActor()))
		);
	}
}

void ADWPlayerController::IssueCommandUnderCursor(bool bAllowInteractCommand)
{
	FHitResult Hit;

	const bool bHit = GetHitResultUnderCursor(ECC_Visibility, true, Hit);

	if (!bHit)
	{
		return;
	}

	AActor* HitActor = Hit.GetActor();

	const bool bIsInteractable = HitActor && HitActor->GetClass()->ImplementsInterface(UDWInteractable::StaticClass());

	if (bIsInteractable)
	{
		if (bAllowInteractCommand || PendingInteractActor == HitActor)
		{
			PendingInteractActor = HitActor;

			const FVector InteractLocation = IDWInteractable::Execute_GetInteractLocation(HitActor);

			MoveToLocation(InteractLocation);
			StartInteractCheckTimer();

			return;
		}
	}

	ClearPendingInteract();
	MoveToLocation(Hit.ImpactPoint);
}

void ADWPlayerController::StartInteractCheckTimer()
{
	if (!GetWorldTimerManager().IsTimerActive(InteractCheckTimerHandle))
	{
		GetWorldTimerManager().SetTimer(InteractCheckTimerHandle, this, &ADWPlayerController::CheckPendingInteract, 0.1f, true);
	}
}

void ADWPlayerController::CheckPendingInteract()
{
	if (!PendingInteractActor)
	{
		ClearPendingInteract();
		return;
	}

	APawn* ControlledPawn = GetPawn();

	if (!ControlledPawn)
	{
		ClearPendingInteract();
		return;
	}

	const float Distance = FVector::Dist2D(ControlledPawn->GetActorLocation(), PendingInteractActor->GetActorLocation());

	if (Distance <= InteractDistance)
	{
		StopMovement();

		IDWInteractable::Execute_Interact(PendingInteractActor, ControlledPawn);

		ClearPendingInteract();
	}
}

void ADWPlayerController::MoveToLocation(const FVector& Location)
{
	if (!GetWorld())
	{
		return;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
		return;
	}

	FNavLocation ProjectedLocation;

	const bool bProjected = NavSys->ProjectPointToNavigation(Location, ProjectedLocation, FVector(300.0f, 300.0f, 500.0f));

	if (!bProjected)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Move failed: target location is not on NavMesh"));
		}
		return;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, ProjectedLocation.Location);
}

void ADWPlayerController::ClearPendingInteract()
{
	PendingInteractActor = nullptr;

	GetWorldTimerManager().ClearTimer(InteractCheckTimerHandle);
}

void ADWPlayerController::SetCharacterMoveSpeed(float NewSpeed)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());

	if (!ControlledCharacter)
	{
		return;
	}

	UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement();

	if (!MoveComp)
	{
		return;
	}

	MoveComp->MaxWalkSpeed = NewSpeed;
}
