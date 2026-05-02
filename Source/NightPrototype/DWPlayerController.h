// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DWInteractionTypes.h"
#include "DWPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UDWInteractionPromptWidget;
class UDWDialogueWidget;
class UDWInteractionOptionsMenuWidget;

/**
 * 
 */
UCLASS()
class NIGHTPROTOTYPE_API ADWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADWPlayerController();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 MappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SetDestinationAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance = 180.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MaxInteractHeightDifference = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float PendingInteractTimeout = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float HoldToActionThreshold = 0.18f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 650.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback")
	float CursorTraceDistance = 100000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback")
	TSubclassOf<AActor> ClickIndicatorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction|UI")
	TSubclassOf<UDWInteractionPromptWidget> InteractionPromptWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction|UI")
	TSubclassOf<UDWInteractionOptionsMenuWidget> InteractionOptionsMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialogue|UI")
	TSubclassOf<UDWDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY()
	UDWInteractionPromptWidget* InteractionPromptWidget = nullptr;
	
	UPROPERTY()
	AActor* FocusedInteractActor = nullptr;

	UPROPERTY()
	AActor* PendingInteractActor = nullptr;

	UPROPERTY()
	AActor* InteractionOptionsMenuActor = nullptr;
	
	UPROPERTY()
	UDWDialogueWidget* DialogueWidget = nullptr;

	UPROPERTY()
	UDWInteractionOptionsMenuWidget* InteractionOptionsMenuWidget = nullptr;
	
	FVector PendingInteractLocation = FVector::ZeroVector;
	
	EDWInteractionAction PendingInteractionAction = EDWInteractionAction::Primary;

	FTimerHandle InteractCheckTimerHandle;

	bool bIsDestinationHeld = false;
	bool bIsActionHoldMode = false;
	float DestinationHoldStartTime = 0.0f;

	void HandleDestinationStarted();
	void HandleDestinationCompleted();

	void IssueCommandUnderCursor(bool bAllowInteractCommand);
	void MoveDirectlyTowardCursor();

	void EnterActionHoldMode();
	void ExitActionHoldMode();

	void CheckPendingInteract();
	void StartInteractCheckTimer();

	bool MoveToLocation(const FVector& Location);
	void ClearPendingInteract();
	
	float PendingInteractStartTime = 0.0f;
	
	void FaceActorsTowardEachOther(AActor* FirstActor, AActor* SecondActor);
	
	void ShowDialogue(const FText& DialogueText);
	void HideDialogue();

	bool bIsInteractionOptionsMenuOpen = false;
	
	void ShowInteractionOptionsMenu(AActor* InteractableActor, const TArray<FDWInteractionOption>& Options);
	void HideInteractionOptionsMenu();

	UFUNCTION()
	void HandleInteractionOptionSelected(FDWInteractionOption Option);


	void SetCharacterMoveSpeed(float NewSpeed);
	
	void UpdateInteractionFocus();
	void SetFocusedInteractActor(AActor* NewFocusedActor);
	void SpawnClickIndicator(const FVector& Location);
	bool TraceCursor(FHitResult& OutHit);
	bool IsUsableInteractable(AActor* Actor);
	
	FVector2D GetClampedMenuPosition(const FVector2D& AnchorPosition, const FVector2D& MenuSize) const
	;
};
