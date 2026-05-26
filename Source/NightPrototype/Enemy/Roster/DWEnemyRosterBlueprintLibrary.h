// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWEnemyRosterBlueprintLibrary.generated.h"

class UDWEnemyRosterSubsystem;

UCLASS()
class NIGHTPROTOTYPE_API UDWEnemyRosterBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Enemy Roster", meta = (WorldContext = "WorldContextObject"))
	static UDWEnemyRosterSubsystem* GetEnemyRoster(const UObject* WorldContextObject);
};
