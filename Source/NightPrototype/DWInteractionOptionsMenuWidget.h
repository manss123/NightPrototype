// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DWInteractionTypes.h"
#include "DWInteractionOptionsMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDWInteractionOptionSelectedSignature, FDWInteractionOption, Option);
/**
 * 
 */
UCLASS()
class NIGHTPROTOTYPE_API UDWInteractionOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FDWInteractionOptionSelectedSignature OnOptionSelectedNative;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void NotifyOptionSelected(FDWInteractionOption Option);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void SetMenuPosition(const FVector2D& ScreenPosition);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void SetOptions(const TArray<FDWInteractionOption>& Options);
};
