// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "DWDialogueTypes.h"
#include "DWDialogueBankDataAsset.h"
#include "Memory/DWMemoryComponent.h"
#include "DWDialogueSelectorLibrary.generated.h"

class UDWGameInstance;

UCLASS()
class NIGHTPROTOTYPE_API UDWDialogueSelectorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	static bool SelectBestDialogueLine(
		UDWDialogueBankDataAsset* DialogueBank, 
		const FGameplayTagContainer& WorldTags,
		UDWMemoryComponent* MemoryComponent,
		const FGameplayTagContainer& ContextTags,
		UDWGameInstance* GameInstance,
		FDWDialogueLine& OutLine
		);
};
