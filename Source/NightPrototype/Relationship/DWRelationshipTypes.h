// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWRelationshipTypes.generated.h"

USTRUCT(BlueprintType)
struct FDWRelationshipEdge
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	FName FromNPCId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	FName ToNPCId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	float Trust = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	float Fear = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	float Respect = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	float Resentment = 0.0f;
};
