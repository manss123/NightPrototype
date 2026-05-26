// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWDialogueTypes.h"
#include "DWDialogueBankDataAsset.generated.h"

UCLASS(BlueprintType)
class NIGHTPROTOTYPE_API UDWDialogueBankDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDWDialogueLine> Lines;
};
