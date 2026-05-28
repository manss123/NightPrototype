// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "DWWorldEventTypes.h"
#include "DWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTPROTOTYPE_API UDWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Memory")
	TSet<FName> WorldEventTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Memory")
	FGameplayTagContainer WorldEventGameplayTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Memory")
	TArray<FDWWorldEvent> WorldEvents;
	
public:
	static const FName Event_PlayerInspectedCorpse;
	static const FName Event_PlayerReadNoticeBoard;

	static const FName Event_PlayerSawVampire;
	static const FName Encounter_VampireRaid;
	
	static const FName Event_PlayerFled;
	static const FName Event_PlayerDefeated;
	static const FName Event_EnemyKilled;
	static const FName Event_EnemyEscaped;
	static const FName Event_VillagerTaken;
	static const FName Event_VillagerSaved;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory")
	void AddWorldEvent(FName EventTag);
	
	UFUNCTION(BlueprintPure, Category = "World Memory")
	bool HasWorldEvent(FName EventTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory")
	void ClearWorldEvents();
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Gameplay Tags")
	void AddWorldGameplayTag(FGameplayTag EventTag);
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Gameplay Tags")
	bool HasWorldGameplayTag(FGameplayTag EventTag) const;
	
	UFUNCTION(BlueprintPure, Category = "World Events")
	FGameplayTagContainer GetWorldGameplayTags() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void AddWorldEventRecord(const FDWWorldEvent& WorldEvent);
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	const TArray<FDWWorldEvent>& GetWorldEventRecords() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void MarkPlayerInspectedCorpse();
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasPlayerInspectedCorpse() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void MarkPlayerReadNoticeBoard();
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void MarkPlayerSawVampire();
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasPlayerSawVampire() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasPlayerReadNoticeBoard() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Debug")
	int32 GetWorldEventCount() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Debug")
	TArray<FName> GetWorldEvents() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Debug")
	void PrintWorldEvents() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Debug")
	void PrintWorldEventRecords() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasVampireRaidStarted() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasWorldEventRecordWithThreat(FGameplayTag EventTag, FGameplayTag ThreatTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	bool HasWorldEventRecordMatching(
		FGameplayTag EventTag,
		FGameplayTag EncounterTag,
		FGameplayTag ThreatTag,
		FGameplayTag LocationTag
	) const;
};
