// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DWEncounterTemplateDataAsset.h"
#include "DWEncounterTypes.h"
#include "GameplayTagContainer.h"
#include "DWEncounterDirector.generated.h"

UCLASS()
class NIGHTPROTOTYPE_API ADWEncounterDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	ADWEncounterDirector();
	
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	bool EvaluateEncountersWithGameplayContext(const FGameplayTagContainer& ContextTags);
	
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	bool TryStartEncounter(UDWEncounterTemplateDataAsset* EncounterData);
	
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	bool CanStartEncounter(UDWEncounterTemplateDataAsset* EncounterData) const;

	UFUNCTION(BlueprintCallable, Category = "Encounter")
	bool ResolveActiveEncounter(FName EncounterId, FGameplayTag OutcomeGameplayTag);

	UFUNCTION(BlueprintCallable, Category = "Encounter|Debug")
	void PrintActiveEncounters() const;

	UFUNCTION(BlueprintCallable, Category = "Encounter")
	bool EvaluateEncountersWithGameplayContextAndTime(const FGameplayTagContainer& ContextTags, int32 CurrentDay, float CurrentHour);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Debug")
	bool bShowDebugMessages = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Debug")
	bool bPrintWorldEventsOnStart = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Encounter")
	TArray<FDWEncounterInstance> ActiveEncounters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	TArray<UDWEncounterTemplateDataAsset*> EncounterTemplates;

	UDWEncounterTemplateDataAsset* FindBestEligibleEncountersByGameplayContext(const FGameplayTagContainer& ContextTags) const;
	
	bool TryStartEncounterWithTime(UDWEncounterTemplateDataAsset* EncounterData, int32 CurrentDay, float CurrentHour);
	
	bool StartEncounterInternal(UDWEncounterTemplateDataAsset* EncounterData, int32 CurrentDay, float CurrentHour);
	
private:
	void ApplyNemesisForOutcome(const FDWEncounterInstance& Instance, FGameplayTag OutcomeGameplayTag);
};
