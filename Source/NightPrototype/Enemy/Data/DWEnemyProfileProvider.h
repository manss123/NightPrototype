// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWEnemyProfileProvider.generated.h"

class UDWEnemyProfileDataAsset;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class NIGHTPROTOTYPE_API UDWEnemyProfileProvider : public UInterface
{
	GENERATED_BODY()
};

class NIGHTPROTOTYPE_API IDWEnemyProfileProvider
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Profile")
	UDWEnemyProfileDataAsset* GetEnemyProfile() const;
};
