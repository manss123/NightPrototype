// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWNemesisBlueprintLibrary.generated.h"

class UDWNemesisSubsystem;

UCLASS()
class NIGHTPROTOTYPE_API UDWNemesisBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Nemesis", meta = (WorldContext = "WorldContextObject"))
	static UDWNemesisSubsystem* GetNemesisSubsystem(const UObject* WorldContextObject);
};
