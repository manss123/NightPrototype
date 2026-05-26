// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DWNemesisTypes.h"
#include "DWNemesisSubsystem.generated.h"

UCLASS()
class NIGHTPROTOTYPE_API UDWNemesisSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Nemesis")
	void SetNemesisState(const FDWNemesisState& state);
	
	UFUNCTION(BlueprintPure, Category = "Nemesis")
	bool GetNemesisState(FName EnemyId, FDWNemesisState& OutState) const;
	
	UFUNCTION(BlueprintCallable, Category = "Nemesis")
	void MarkEnemyDefeatedPlayer(FName EnemyId);
	
	UFUNCTION(BlueprintCallable, Category = "Nemesis")
	void MarkEnemyDefeatedByPlayer(FName EnemyId);
	
	UFUNCTION(BlueprintCallable, Category = "Nemesis")
	void PrintNemesisStates() const;
	
	UFUNCTION(BlueprintCallable, Category = "Nemesis")
	void ApplyEnemyMemoryForNemesis(UObject* WorldContextObject, FName EnemyId);
private:
	UPROPERTY()
	TArray<FDWNemesisState> NemesisStates;
	
	int32 FindNemesisStateIndex(FName EnemyId) const;
};
