// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWNPCProfileProvider.generated.h"

class UDWNPCProfileDataAsset;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class NIGHTPROTOTYPE_API UDWNPCProfileProvider : public UInterface
{
	GENERATED_BODY()
};

class NIGHTPROTOTYPE_API IDWNPCProfileProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Profile")
	UDWNPCProfileDataAsset* GetNPCProfile() const;
};
