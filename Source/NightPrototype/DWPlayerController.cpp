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
#include "DWDialogueWidget.h"
#include "DWInteractionOptionsMenuWidget.h"

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
	
	if (InteractionOptionsMenuWidgetClass)
	{
		InteractionOptionsMenuWidget = CreateWidget<UDWInteractionOptionsMenuWidget>(this, InteractionOptionsMenuWidgetClass);
		
		if (InteractionOptionsMenuWidget)
		{
			InteractionOptionsMenuWidget->AddToViewport();
			InteractionOptionsMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			
			InteractionOptionsMenuWidget->OnOptionSelectedNative.AddDynamic(this, &ADWPlayerController::HandleInteractionOptionSelected);
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
	
	if (FocusedInteractActor && InteractionPromptWidget && !bIsInteractionOptionsMenuOpen)
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
	
	if (bIsInteractionOptionsMenuOpen)
	{
		HideInteractionOptionsMenu();
		ClearPendingInteract();
		return;
	}
	
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
			const TArray<FDWInteractionOption> Options = IDWInteractable::Execute_GetInteractionOptions(HitActor);

			if (Options.Num() > 1)
			{
				ClearPendingInteract();
				StopMovement();
				
				if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
				{
					if (UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement())
					{
						MoveComp->StopMovementImmediately();
					}
				}
				ShowInteractionOptionsMenu(HitActor, Options);
				return;
			}

			PendingInteractActor = HitActor;
			PendingInteractLocation = IDWInteractable::Execute_GetInteractLocation(HitActor);
			PendingInteractionAction = Options.Num() > 0 ? Options[0].Action : EDWInteractionAction::Primary;
			PendingInteractStartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

			if (MoveToLocation(PendingInteractLocation))
			{
				StartInteractCheckTimer();
			}
			else
			{
				ClearPendingInteract();
			}

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
	
	if (GetWorld() && PendingInteractTimeout > 0.0f)
	{
		const float ElapsedTime = GetWorld()->GetTimeSeconds() - PendingInteractStartTime;
		
		if (ElapsedTime > PendingInteractTimeout)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Pending interact timed out"));
			}
			ClearPendingInteract();
			return;
		}
	}
	
	if (bIsDestinationHeld && !bIsActionHoldMode)
	{
		return;
	}

	const FVector targetLocation = PendingInteractLocation;
	
	const FVector PawnFloorLocation = IDWInteractable::GetActorFloorLocation(ControlledPawn);
	
	float Distance = FVector::Dist2D(PawnFloorLocation, targetLocation);
	
	float TargetFloorZ = targetLocation.Z;
	
	if (Cast<ACharacter>(PendingInteractActor))
	{
		TargetFloorZ = IDWInteractable::GetActorFloorLocation(PendingInteractActor).Z;
	}
	
	const float HeightDifference = FMath::Abs(PawnFloorLocation.Z - TargetFloorZ);
	
	if (HeightDifference > MaxInteractHeightDifference)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Too height"));
		}
		return;
	}
	
	const float ActorInteractDistance = IDWInteractable::Execute_GetInteractDistance(PendingInteractActor);
	const float EffectiveInteractDistance = ActorInteractDistance > 0.0f ? ActorInteractDistance : InteractDistance;

	if (Distance <= EffectiveInteractDistance)
	{
		StopMovement();
		
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			if (UCharacterMovementComponent* MoveComp = ControlledCharacter->GetCharacterMovement())
			{
				MoveComp->StopMovementImmediately();
			}
		}

		if (IDWInteractable::Execute_ShouldFaceInteractor(PendingInteractActor))
		{
			FaceActorsTowardEachOther(ControlledPawn, PendingInteractActor);
		}
		
		IDWInteractable::Execute_InteractWithOption(PendingInteractActor, ControlledPawn, PendingInteractionAction);

		if (IDWInteractable::Execute_ShouldShowDialogue(PendingInteractActor))
		{
			const FText DialogueText = IDWInteractable::Execute_GetDialogueText(PendingInteractActor);
		
			if (!DialogueText.IsEmpty())
			{
				ShowDialogue(DialogueText);
			}
		}

		ClearPendingInteract();
	}
}

bool ADWPlayerController::MoveToLocation(const FVector& Location)
{
	if (!GetWorld())
	{
		return false;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
		return true;
	}

	FNavLocation ProjectedLocation;

	const bool bProjected = NavSys->ProjectPointToNavigation(Location, ProjectedLocation, FVector(300.0f, 300.0f, 500.0f));

	if (!bProjected)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Move failed: target location is not on NavMesh"));
		}
		return false;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, ProjectedLocation.Location);
	return true;
}

void ADWPlayerController::ClearPendingInteract()
{
	PendingInteractActor = nullptr;
	PendingInteractionAction = EDWInteractionAction::Primary;
	PendingInteractLocation = FVector::ZeroVector;
	PendingInteractStartTime = 0.0f;

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
			if (bIsInteractionOptionsMenuOpen)
			{
				InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
				return;
			}
			
			FText PromptText = IDWInteractable::Execute_GetInteractText(FocusedInteractActor);
			
			const TArray<FDWInteractionOption> Options = IDWInteractable::Execute_GetInteractionOptions(FocusedInteractActor);
			
			if (Options.Num() > 0 && !Options[0].Label.IsEmpty())
			{
				PromptText = Options[0].Label;
			}
			
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

void ADWPlayerController::ShowInteractionOptionsMenu(AActor* InteractableActor, const TArray<FDWInteractionOption>& Options)
{
	if (!InteractionOptionsMenuWidget)
	{
		return;
	}
	
	InteractionOptionsMenuActor = InteractableActor;
	
	bIsInteractionOptionsMenuOpen = true;
	
	if (InteractionPromptWidget)
	{
		InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	const FVector2D MenuSize(160.0f, Options.Num() * 42.0f + 16.0f);
	const FVector2D MenuPosition = GetClampedMenuPosition(MousePosition, MenuSize);
	
	InteractionOptionsMenuWidget->SetOptions(Options);
	InteractionOptionsMenuWidget->SetMenuPosition(MenuPosition);
	InteractionOptionsMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void ADWPlayerController::HideInteractionOptionsMenu()
{
	bIsInteractionOptionsMenuOpen = false;
	InteractionOptionsMenuActor = nullptr;
	ClearPendingInteract();
	
	if (!InteractionOptionsMenuWidget)
	{
		return;
	}

	InteractionOptionsMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ADWPlayerController::HandleInteractionOptionSelected(FDWInteractionOption Option)
{
	AActor* SelectedActor = InteractionOptionsMenuActor;
	
	HideInteractionOptionsMenu();
	
	bIsDestinationHeld = false;
	bIsActionHoldMode = false;
	
	if (!IsUsableInteractable(SelectedActor))
	{
		return;
	}
	
	PendingInteractActor = SelectedActor;
	PendingInteractLocation = IDWInteractable::Execute_GetInteractLocation(SelectedActor);
	PendingInteractionAction = Option.Action;
	PendingInteractStartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	if (MoveToLocation(PendingInteractLocation))
	{
		StartInteractCheckTimer();
	}
	else
	{
		ClearPendingInteract();
	}
}

FVector2D ADWPlayerController::GetClampedMenuPosition(const FVector2D& AnchorPosition, const FVector2D& MenuSize) const
{
	FVector2D ViewPortSize(0.0f, 0.0f);
	
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	
	const float Margin = 8.0f;
	const float Offset = 12.0f;
	
	FVector2D MenuPosition = AnchorPosition + FVector2D(Offset, Offset);
	
	if (MenuPosition.X + MenuSize.X + Margin > ViewPortSize.X)
	{
		MenuPosition.X = AnchorPosition.X - MenuSize.X - Offset;
	}
	
	if (MenuPosition.Y + MenuSize.Y + Margin > ViewPortSize.Y)
	{
		MenuPosition.Y = AnchorPosition.Y - MenuSize.Y - Offset;
	}
	
	MenuPosition.X = FMath::Clamp(MenuPosition.X, Margin, ViewPortSize.X - MenuSize.X - Margin);
	MenuPosition.Y = FMath::Clamp(MenuPosition.Y, Margin, ViewPortSize.Y - MenuSize.Y - Margin);
	
	return MenuPosition;
}
