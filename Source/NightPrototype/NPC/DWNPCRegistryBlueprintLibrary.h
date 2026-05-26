// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWNPCRegistryBlueprintLibrary.generated.h"

class UDWNPCRegistrySubsystem;

UCLASS()
class NIGHTPROTOTYPE_API UDWNPCRegistryBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "NPC Registry", meta = (WorldContext = "WorldContextObject"))
	static UDWNPCRegistrySubsystem* GetNPCRegistry(const UObject* WorldContextObject);
};
