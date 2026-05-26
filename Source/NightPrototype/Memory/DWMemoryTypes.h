// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWMemoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FDWMemoryEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGuid MemoryId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGameplayTag MemoryTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGameplayTag SourceEventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGameplayTagContainer ContextTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGameplayTag LocationTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FName SourceActorId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	float GameHour = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	float Strength = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	bool bIsPersonal = false;
};
