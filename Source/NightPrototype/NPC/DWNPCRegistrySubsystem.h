// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "DWNPCRegistrySubsystem.generated.h"

class UDWNPCProfileDataAsset;

UCLASS()
class NIGHTPROTOTYPE_API UDWNPCRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "NPC Registry")
	void RegisterNPC(AActor* NPCActor);
	
	UFUNCTION(BlueprintCallable, Category = "NPC Registry")
	void UnregisterNPC(AActor* NPCActor);
	
	UFUNCTION(BlueprintCallable, Category = "NPC Registry")
	TArray<AActor*> GetRegisteredNPCs() const;
	
	UFUNCTION(BlueprintPure, Category = "NPC Registry")
	AActor* FindNPCById(FName NPCId) const;

	UFUNCTION(BlueprintPure, Category = "NPC Registry")
	TArray<AActor*> FindNPCsByRole(FGameplayTag RoleTag) const;
	
	UFUNCTION(BlueprintPure, Category = "NPC Registry")
	TArray<AActor*> FindNPCsByLocation(FGameplayTag LocationTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "NPC Registry|Debug")
	void PrintRegisteredNPCs() const;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> RegisteredNPCs;
};
