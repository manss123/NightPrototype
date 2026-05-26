// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DWEnemyProfileDataAsset.generated.h"

UCLASS(BlueprintType)
class NIGHTPROTOTYPE_API UDWEnemyProfileDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Profile")
	FName EnemyId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Profile")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Profile")
	FGameplayTag ThreatTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Profile")
	FGameplayTag EnemyTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Profile")
	TSubclassOf<AActor> EnemyActorClass;
};
