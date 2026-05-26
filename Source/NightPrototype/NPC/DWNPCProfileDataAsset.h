// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DWNPCProfileDataAsset.generated.h"

class UDWDialogueBankDataAsset;

UCLASS(BlueprintType)
class NIGHTPROTOTYPE_API UDWNPCProfileDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Profile")
	FName NPCId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Profile")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Profile")
	FGameplayTag LocationTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Profile")
	FGameplayTag RoleTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Profile")
	TObjectPtr<UDWDialogueBankDataAsset> DialogueBank;
};
