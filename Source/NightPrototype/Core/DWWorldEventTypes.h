// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWWorldEventTypes.generated.h"

USTRUCT(BlueprintType)
struct FDWWorldEvent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGuid EventId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTag EventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTag EncounterTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTag ThreatTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTagContainer ContextTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTag LocationTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FGameplayTag InstigatorTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	FName SourceActorId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	float GameHour = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
	int32 GameDay = 1;
};
