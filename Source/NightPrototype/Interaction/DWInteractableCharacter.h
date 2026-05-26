// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWInteractable.h"
#include "GameFramework/Character.h"
#include "DWInteractableCharacter.generated.h"

UCLASS()
class NIGHTPROTOTYPE_API ADWInteractableCharacter : public ACharacter, public IDWInteractable
{
	GENERATED_BODY()

public:
	ADWInteractableCharacter();
	
	virtual TArray<FDWInteractionOption> GetInteractionOptions_Implementation() override;
	virtual FVector GetInteractLocation_Implementation() override;
	virtual FText GetInteractText_Implementation() override;
	virtual float GetInteractDistance_Implementation() override;
	virtual bool CanInteract_Implementation(AActor* Interactor) override;
	virtual bool ShouldFaceInteractor_Implementation() override;
	virtual bool ShouldShowDialogue_Implementation() override;
	virtual FText GetDialogueText_Implementation() override;
	virtual void InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action) override;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FVector GetInteractLocationForInteractor(AActor* Interactor);
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractText = FText::FromString("Interact");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<FDWInteractionOption> InteractionOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShouldFaceInteractor = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<USceneComponent> InteractionPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bUseDynamicInteractLocation = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (EditCondition = "bUseDynamicInteractLocation", ClampMin = "0.0"))
	float DynamicInteractDistance = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	bool bShowDialogueOnInteract = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	FText DialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	bool bShowDebugInteractionMessage = false;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
