// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DWEncounterOutcomeComponent.generated.h"

class UDWBodyHealthComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWEncounterOutcomeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWEncounterOutcomeComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Outcome")
	FName EncounterId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Outcome")
	FGameplayTag OutcomeOnDeath;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Outcome")
	FGameplayTag OutcomeOnDowned;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Outcome")
	bool bResolveOnDeath = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Outcome")
	bool bResolveOnDowned = false;
	
private:
	UFUNCTION()
	void HandleOwnerDeath();
	
	UFUNCTION()
	void HandleOwnerDowned();
	
	void ResolveOutcome(FGameplayTag OutcomeTag);
	
	bool bHasResolvedOutcome = false;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
