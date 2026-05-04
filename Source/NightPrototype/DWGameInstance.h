// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	
public:
	static const FName Event_PlayerInspectedCorpse;
	static const FName Event_PlayerReadNoticeBoard;
	
	static const FName Event_FirstNightStarted;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory")
	void AddWorldEvent(FName EventTag);
	
	UFUNCTION(BlueprintPure, Category = "World Memory")
	bool HasWorldEvent(FName EventTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory")
	void ClearWorldEvents();
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void MarkPlayerInspectedCorpse();
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasPlayerInspectedCorpse() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Events")
	void MarkPlayerReadNoticeBoard();
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Events")
	bool HasPlayerReadNoticeBoard() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Debug")
	int32 GetWorldEventCount() const;
	
	UFUNCTION(BlueprintPure, Category = "World Memory|Debug")
	TArray<FName> GetWorldEvents() const;
	
	UFUNCTION(BlueprintCallable, Category = "World Memory|Debug")
	void PrintWorldEvents() const;
};
