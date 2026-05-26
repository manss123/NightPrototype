// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Relationship/DWRelationshipTypes.h"
#include "DWRelationshipSubsystem.generated.h"

UCLASS()
class NIGHTPROTOTYPE_API UDWRelationshipSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Relationship")
	void SetRelationship(const FDWRelationshipEdge& Relationship);
	
	UFUNCTION(BlueprintPure, Category = "Relationship")
	bool GetRelationship(FName FromNPCId, FName ToNPCId, FDWRelationshipEdge& OutRelationship) const;
	
	UFUNCTION(BlueprintCallable, Category = "Relationship")
	void AdjustTrust(FName FromNPCId, FName ToNPCId, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Relationship")
	void AdjustFear(FName FromNPCId, FName ToNPCId, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Relationship")
	void AdjustRespect(FName FromNPCId, FName ToNPCId, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Relationship")
	void AdjustmentResentment(FName FromNPCId, FName ToNPCId, float Delta);
	
	UFUNCTION(BlueprintPure, Category = "Relationship")
	float GetTrust(FName FromNPCId, FName ToNPCId) const;
	
	UFUNCTION(BlueprintPure, Category = "Relationship")
	float GetFear(FName FromNPCId, FName ToNPCId) const;
	
	UFUNCTION(BlueprintPure, Category = "Relationship")
	float GetRespect(FName FromNPCId, FName ToNPCId) const;
	
	UFUNCTION(BlueprintPure, Category = "Relationship")
	float GetResentment(FName FromNPCId, FName ToNPCId) const;
	
	UFUNCTION(BlueprintCallable, Category = "Relationship|Debug")
	void PrintRelationships() const;
	
private:
	UPROPERTY()
	TArray<FDWRelationshipEdge> Relationships;
	
	int32 FindRelationshipIndex(FName FromNPCId, FName ToNPCId) const;
};
