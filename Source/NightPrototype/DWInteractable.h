// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWInteractionTypes.h"
#include "DWInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDWInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NIGHTPROTOTYPE_API IDWInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	TArray<FDWInteractionOption> GetInteractionOptions();
	
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void InteractWithOption(AActor* Interactor, EDWInteractionAction Action);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FVector GetInteractLocation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractText();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetDialogueText();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	float GetInteractDistance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool ShouldFaceInteractor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool ShouldShowDialogue();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Feedback")
	void OnInteractFocusBegin(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Feedback")
	void OnInteractFocusEnd(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Feedback")
	bool CanInteract(AActor* Interactor);

	virtual bool CanInteract_Implementation(AActor* Interactor);
	virtual  FText GetDialogueText_Implementation();
	virtual void OnInteractFocusBegin_Implementation(AActor* Interactor);
	virtual void OnInteractFocusEnd_Implementation(AActor* Interactor);
	virtual float GetInteractDistance_Implementation();
	virtual bool ShouldFaceInteractor_Implementation();
	virtual bool ShouldShowDialogue_Implementation();
	virtual TArray<FDWInteractionOption> GetInteractionOptions_Implementation();
	virtual void InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Option);
};
