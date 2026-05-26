// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWRelationshipBlueprintLibrary.generated.h"

class UDWRelationshipSubsystem;

UCLASS()
class NIGHTPROTOTYPE_API UDWRelationshipBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Relationship", meta = (WorldContext = "WorldContextObject"))
	static UDWRelationshipSubsystem* GetRelationshipSubsystem(const UObject* WorldContextObject);
};
