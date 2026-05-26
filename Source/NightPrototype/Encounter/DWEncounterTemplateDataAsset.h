// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWEncounterTypes.h"
#include "DWEncounterTemplateDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NIGHTPROTOTYPE_API UDWEncounterTemplateDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
	FDWEncounterTemplate EncounterTemplate;
};
