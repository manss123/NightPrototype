// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DWInteractionPromptWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTPROTOTYPE_API UDWInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category= "Interaction")
	void SetPromptText(const FText& NewText);
	
	UFUNCTION(BlueprintImplementableEvent, Category= "Interaction")
	void SetPromptPosition(const FVector2D& ScreenPosition);
};
