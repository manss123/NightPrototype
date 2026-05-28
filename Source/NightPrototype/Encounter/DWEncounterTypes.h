// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWEncounterTypes.generated.h"

class UDWEnemyProfileDataAsset;
class AActor;

USTRUCT(BlueprintType)
struct FDWEncounterTemplate
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FName EncounterId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	bool bOneShot = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	float CooldownSeconds = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	int32 Priority = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	TObjectPtr<UDWEnemyProfileDataAsset> PrimaryEnemyProfile;;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag EncounterGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag StartedEventGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag LocationGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag ThreatGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTagContainer RequiredWorldGameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTagContainer BlockedWorldGameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTagContainer ContextGameplayTags;
};

USTRUCT(BlueprintType)
struct FDWEncounterInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
	FGuid InstanceId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FName EncounterId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FName PrimaryEnemyId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FName VictimNPCId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	FVector EncounterLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Encounter")
	bool bResolved = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Encounter")
	TObjectPtr<AActor> PrimaryEnemyActor = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Encounter")
	TObjectPtr<AActor> VictimActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Time")
	int32 StartGameDay = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Time")
	float StartGameHour = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag EncounterGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag StartedEventGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag OutcomeGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTagContainer ContextGameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag LocationGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Gameplay Tags")
	FGameplayTag ThreatGameplayTag;
};
