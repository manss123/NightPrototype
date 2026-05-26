// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "DWEnemyRosterSubsystem.generated.h"

class UDWEnemyProfileDataAsset;

UCLASS()
class NIGHTPROTOTYPE_API UDWEnemyRosterSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION( BlueprintCallable, Category = "Enemy Roster")
	void RegisterEnemy(AActor* EnemyActor);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Roster")
	void UnregisterEnemy(AActor* EnemyActor);
	
	UFUNCTION(BlueprintPure, Category = "Enemy Roster")
	TArray<AActor*> GetRegisteredEnemies() const;
	
	UFUNCTION(BlueprintPure, Category = "Enemy Roster")
	AActor* FindEnemyById(FName EnemyId) const;
	
	UFUNCTION(BlueprintPure, Category = "Enemy Roster")
	TArray<AActor*> FindEnemyByThreat(FGameplayTag ThreatTag) const;
	
	UFUNCTION(BlueprintPure, Category = "Enemy Roster")
	TArray<AActor*> FindEnemyByType(FGameplayTag EnemyTypeTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Roster|Debug")
	void PrintRegisteredEnemies() const;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> RegisteredEnemies;
};
