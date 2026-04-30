// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DWPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UDWInteractionPromptWidget;

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
	
	UPROPERTY()
	UDWInteractionPromptWidget* InteractionPromptWidget = nullptr;
	
	UPROPERTY()
	AActor* FocusedInteractActor = nullptr;

	UPROPERTY()
	AActor* PendingInteractActor = nullptr;
	
	FVector PendingInteractLocation = FVector::ZeroVector;

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

	void MoveToLocation(const FVector& Location);
	void ClearPendingInteract();
	
	void FaceActorsTowardEachOther(AActor* FirstActor, AActor* SecondActor);

	void SetCharacterMoveSpeed(float NewSpeed);
	
	void UpdateInteractionFocus();
	void SetFocusedInteractActor(AActor* NewFocusedActor);
	void SpawnClickIndicator(const FVector& Location);
	bool TraceCursor(FHitResult& OutHit);
	bool IsUsableInteractable(AActor* Actor);
};
