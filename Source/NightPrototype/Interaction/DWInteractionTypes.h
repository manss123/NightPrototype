// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWInteractionTypes.generated.h"

UENUM(BlueprintType)
enum  class EDWInteractionAction : uint8
{
	Primary UMETA(DisplayName = "Primary"),
	Talk UMETA(DisplayName = "Talk"),
	Open UMETA(DisplayName = "Open"),
	Inspect UMETA(DisplayName = "Inspect"),
	PickUp UMETA(DisplayName = "Pick Up"),
	Attack UMETA(DisplayName = "Attack"),
	Rest UMETA(DisplayName = "Rest")
};

USTRUCT(BlueprintType)
struct FDWInteractionOption
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EDWInteractionAction Action = EDWInteractionAction::Primary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText Label;
};
