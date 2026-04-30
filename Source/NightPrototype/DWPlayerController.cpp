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
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "DWInteractionPromptWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include  "DWDialogueWidget.h"

ADWPlayerController::ADWPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
	
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
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
	
	if (InteractionPromptWidgetClass)
	{
		InteractionPromptWidget = CreateWidget<UDWInteractionPromptWidget>(this, InteractionPromptWidgetClass);
		
		if (InteractionPromptWidget)
		{
			InteractionPromptWidget->AddToViewport();
			InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UDWDialogueWidget>(this, DialogueWidgetClass);
		
		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
	
	UpdateInteractionFocus();
	
	if (FocusedInteractActor && InteractionPromptWidget)
	{
		const FVector2D MousePosition =  UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		
		FVector2D ViewportSize(0.0f, 0.0f);
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}
		
		const FVector2D PromptSize(120.0f, 40.0f);
		const float Margin = 8.0f;
		const float Offset = 18.0f;
		
		FVector2D PromptPosition = MousePosition + FVector2D(Offset, Offset);
		
		if (PromptPosition.X + PromptSize.X + Margin > ViewportSize.X)
		{
			PromptPosition.X = MousePosition.X - PromptSize.X - Offset;
		}
		
		if (PromptPosition.Y + PromptSize.Y + Margin > ViewportSize.Y)
		{
			PromptPosition.Y = MousePosition.Y - PromptSize.Y - Offset;
		}
		
		PromptPosition.X = FMath::Clamp(PromptPosition.X, Margin, ViewportSize.X - PromptSize.X - Margin);
		PromptPosition.Y = FMath::Clamp(PromptPosition.Y, Margin, ViewportSize.Y - PromptSize.Y - Margin);
		
		InteractionPromptWidget->SetPromptPosition(PromptPosition);
	}

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
	HideDialogue();
	
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

	if (bIsActionHoldMode)
	{
		ExitActionHoldMode();
	}
}

void ADWPlayerController::EnterActionHoldMode()
{
	bIsActionHoldMode = true;

	ClearPendingInteract();

	StopMovement();

	SetIgnoreMoveInput(false);

	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking);

			MoveComp->MaxWalkSpeed = RunSpeed;
		}
	}
}

void ADWPlayerController::ExitActionHoldMode()
{
	bIsActionHoldMode = false;

	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement())
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
}

void ADWPlayerController::IssueCommandUnderCursor(bool bAllowInteractCommand)
{
	FHitResult Hit;

	const bool bHit = GetHitResultUnderCursor(ECC_Visibility, true, Hit);

	if (!bHit)
	{
		return;
	}
	
	SpawnClickIndicator(Hit.ImpactPoint);

	AActor* HitActor = Hit.GetActor();

	if (IsUsableInteractable(HitActor))
	{
		if (bAllowInteractCommand || PendingInteractActor == HitActor)
		{
			PendingInteractActor = HitActor;

			PendingInteractLocation = IDWInteractable::Execute_GetInteractLocation(HitActor);

			MoveToLocation(PendingInteractLocation);
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
	if (!IsUsableInteractable(PendingInteractActor))
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
	
	if (bIsDestinationHeld && !bIsActionHoldMode)
	{
		return;
	}

	const FVector targetLocation = PendingInteractLocation;
	
	float Distance = FVector::Dist2D(ControlledPawn->GetActorLocation(), targetLocation);

	if (Distance <= InteractDistance)
	{
		StopMovement();
		
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			if (UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement())
			{
				MoveComp->StopMovementImmediately();
			}
		}
		
		FaceActorsTowardEachOther(ControlledPawn, PendingInteractActor);

		IDWInteractable::Execute_Interact(PendingInteractActor, ControlledPawn);
		
		const FText DialogueText = IDWInteractable::Execute_GetDialogueText(PendingInteractActor);
		
		if (!DialogueText.IsEmpty())
		{
			ShowDialogue(DialogueText);
		}

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
	PendingInteractLocation = FVector::ZeroVector;

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

// helpers
bool ADWPlayerController::TraceCursor(FHitResult& OutHit)
{
	return GetHitResultUnderCursor(ECC_Visibility, true, OutHit);
}

bool ADWPlayerController::IsUsableInteractable(AActor* Actor)
{
	if (!Actor || !Actor->GetClass()->ImplementsInterface(UDWInteractable::StaticClass()))
	{
		return false;
	}
	
	APawn* ControlledPawn = GetPawn();
	return IDWInteractable::Execute_CanInteract(Actor, ControlledPawn);
}

void ADWPlayerController::UpdateInteractionFocus()
{
	FHitResult Hit;
	AActor* NewFocusedActor = nullptr;
	
	if (TraceCursor(Hit) && IsUsableInteractable(Hit.GetActor()))
	{
		NewFocusedActor = Hit.GetActor();
	}
	
	SetFocusedInteractActor(NewFocusedActor);
	
	CurrentMouseCursor = FocusedInteractActor ? EMouseCursor::Hand : EMouseCursor::Default;
}

void ADWPlayerController::SetFocusedInteractActor(AActor* NewFocusedActor)
{
	if (FocusedInteractActor == NewFocusedActor)
	{
		return;
	}
	
	APawn* ControlledPawn = GetPawn();
	
	if (FocusedInteractActor && FocusedInteractActor->GetClass()->ImplementsInterface(UDWInteractable::StaticClass()))
	{
		IDWInteractable::Execute_OnInteractFocusEnd(FocusedInteractActor, ControlledPawn);
	}
	
	FocusedInteractActor = NewFocusedActor;
	
	if (FocusedInteractActor)
	{
		IDWInteractable::Execute_OnInteractFocusBegin(FocusedInteractActor, ControlledPawn);
		
		if (InteractionPromptWidget)
		{
			const FText PromptText = IDWInteractable::Execute_GetInteractText(FocusedInteractActor);
			InteractionPromptWidget->SetPromptText(PromptText);
			InteractionPromptWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (InteractionPromptWidget)
		{
			InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ADWPlayerController::SpawnClickIndicator(const FVector& Location)
{
	if (!ClickIndicatorClass || !GetWorld())
	{
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	GetWorld()->SpawnActor<AActor>(ClickIndicatorClass, Location, FRotator::ZeroRotator, SpawnParams);
}

void ADWPlayerController::FaceActorsTowardEachOther(AActor* FirstActor, AActor* SecondActor)
{
	if (!FirstActor || !SecondActor)
	{
		return;
	}
	
	const FVector FirstLocation = FirstActor->GetActorLocation();
	const FVector SecondLocation = SecondActor->GetActorLocation();
	
	FVector FirstToSecond = SecondLocation - FirstLocation;
	FirstToSecond.Z = 0.0f;
	
	FVector SecondToFirst = FirstLocation - SecondLocation;
	SecondToFirst.Z = 0.0f;
	
	if (!FirstToSecond.IsNearlyZero())
	{
		const FRotator FirstRotation(0.0f, FirstToSecond.Rotation().Yaw, 0.0f);
		FirstActor->SetActorRotation(FirstRotation);
	}
	
	if (!SecondToFirst.IsNearlyZero())
	{
		const FRotator SecondRotation(0.0f, SecondToFirst.Rotation().Yaw, 0.0f);
		SecondActor->SetActorRotation(SecondRotation);
	}
}

void ADWPlayerController::ShowDialogue(const FText& DialogueText)
{
	if (!DialogueWidget)
	{
		return;
	}
	
	DialogueWidget->SetDialogueText(DialogueText);
	DialogueWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ADWPlayerController::HideDialogue()
{
	if (!DialogueWidget)
	{
		return;
	}
	
	DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
}
