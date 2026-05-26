// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DWInteractable.h"
#include "DWInteractableActor.generated.h"

UCLASS()
class NIGHTPROTOTYPE_API ADWInteractableActor : public AActor, public IDWInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWInteractableActor();
	
	virtual TArray<FDWInteractionOption> GetInteractionOptions_Implementation() override;
	virtual FVector GetInteractLocation_Implementation() override;
	virtual FText GetInteractText_Implementation() override;
	virtual float GetInteractDistance_Implementation() override;
	virtual bool CanInteract_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	bool bShowDialogueOnInteract = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	FText DialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Dialogue")
	bool ShowDebugInteractionMessage = true;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldShowDialogue_Implementation() override;
	virtual FText GetDialogueText_Implementation() override;
	virtual void InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Option) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractText = FText::FromString(TEXT("Interact"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<FDWInteractionOption> InteractionOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<USceneComponent> InteractionPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bUseDynamicInteractLocation = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (EditCondition = "bUseDynamicInteractLocation", ClampMin = "0.0"))
	float DynamicInteractDistance = 120.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FVector GetInteractLocationForInteractor(AActor* Interactor);
};
