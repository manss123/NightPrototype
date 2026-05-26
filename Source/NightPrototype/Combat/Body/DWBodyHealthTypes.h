// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWBodyHealthTypes.generated.h"

UENUM(BlueprintType)
enum class EDWBodyState : uint8
{
	Healthy UMETA(DisplayName = "Healthy"),
	Injured UMETA(DisplayName = "Injured"),
	Downed UMETA(DisplayName = "Downed"),
	Unconscious UMETA(DisplayName = "Unconscious"),
	Dying UMETA(DisplayName = "Dying"),
	Dead UMETA(DisplayName = "Dead"),
};

USTRUCT(BlueprintType)
struct FDWBodyPartHealth
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	FGameplayTag BodyPartTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	float CurrentHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	bool bDeathOnZero = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Health")
	bool bIsDisabled = false;
};