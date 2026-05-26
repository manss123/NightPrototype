// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DWDialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTPROTOTYPE_API UDWDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void SetDialogueText(const FText& NewText);
};
