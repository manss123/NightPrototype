// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DWEnemyMemoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWEnemyMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWEnemyMemoryComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Memory")
	void AddEnemyMemory(FGameplayTag MemoryTag);
	
	UFUNCTION(BlueprintPure, Category = "Enemy Memory")
	bool HasEnemyMemory(FGameplayTag MemoryTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Memory|Debug")
	void PrintEnemyMemories() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Memory", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer EnemyMemoryTags;
};
