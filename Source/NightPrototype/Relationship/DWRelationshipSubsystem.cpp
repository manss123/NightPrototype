// Fill out your copyright notice in the Description page of Project Settings.


#include "DWRelationshipSubsystem.h"
#include "Engine/Engine.h"

void UDWRelationshipSubsystem::SetRelationship(const FDWRelationshipEdge& Relationship)
{
	if (Relationship.FromNPCId.IsNone() || Relationship.ToNPCId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindRelationshipIndex(Relationship.FromNPCId, Relationship.ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		Relationships[ExistingIndex] = Relationship;
		return;
	}
	
	Relationships.Add(Relationship);
}

bool UDWRelationshipSubsystem::GetRelationship(FName FromNPCId, FName ToNPCId, FDWRelationshipEdge& OutRelationship) const
{
	const int32 ExistingIndex = FindRelationshipIndex(FromNPCId, ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		return false;
	}
	
	OutRelationship = Relationships[ExistingIndex];
	return true;
}

void UDWRelationshipSubsystem::AdjustTrust(FName FromNPCId, FName ToNPCId, float Delta)
{
	if (FromNPCId.IsNone() || ToNPCId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindRelationshipIndex(FromNPCId, ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		Relationships[ExistingIndex].Trust = FMath::Clamp(Relationships[ExistingIndex].Trust + Delta, -100.0f, 100.0f);
		return;
	}
	
	FDWRelationshipEdge NewRelationship;
	NewRelationship.FromNPCId = FromNPCId;
	NewRelationship.ToNPCId = ToNPCId;
	NewRelationship.Trust = FMath::Clamp(Delta, -100.0f, 100.0f);
	Relationships.Add(NewRelationship);
}

void UDWRelationshipSubsystem::AdjustFear(FName FromNPCId, FName ToNPCId, float Delta)
{
	if (FromNPCId.IsNone() || ToNPCId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindRelationshipIndex(FromNPCId, ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		Relationships[ExistingIndex].Fear = FMath::Clamp(Relationships[ExistingIndex].Fear + Delta, -100.0f, 100.0f);
		return;
	}
	
	FDWRelationshipEdge NewRelationship;
	NewRelationship.FromNPCId = FromNPCId;
	NewRelationship.ToNPCId = ToNPCId;
	NewRelationship.Fear = FMath::Clamp(Delta, -100.0f, 100.0f);;
	Relationships.Add(NewRelationship);
}

void UDWRelationshipSubsystem::AdjustRespect(FName FromNPCId, FName ToNPCId, float Delta)
{
	if (FromNPCId.IsNone() || ToNPCId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindRelationshipIndex(FromNPCId, ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		Relationships[ExistingIndex].Respect = FMath::Clamp(Relationships[ExistingIndex].Respect + Delta, -100.0f, 100.0f);
		return;
	}

	FDWRelationshipEdge NewRelationship;
	NewRelationship.FromNPCId = FromNPCId;
	NewRelationship.ToNPCId = ToNPCId;
	NewRelationship.Respect = FMath::Clamp(Delta, -100.0f, 100.0f);
	Relationships.Add(NewRelationship);
}

void UDWRelationshipSubsystem::AdjustmentResentment(FName FromNPCId, FName ToNPCId, float Delta)
{
	if (FromNPCId.IsNone() || ToNPCId.IsNone())
	{
		return;
	}

	const int32 ExistingIndex = FindRelationshipIndex(FromNPCId, ToNPCId);
	if (ExistingIndex != INDEX_NONE)
	{
		Relationships[ExistingIndex].Resentment = FMath::Clamp(Relationships[ExistingIndex].Resentment + Delta, -100.0f, 100.0f);
		return;
	}

	FDWRelationshipEdge NewRelationship;
	NewRelationship.FromNPCId = FromNPCId;
	NewRelationship.ToNPCId = ToNPCId;
	NewRelationship.Resentment = FMath::Clamp(Delta, -100.0f, 100.0f);
	Relationships.Add(NewRelationship);
}

float UDWRelationshipSubsystem::GetTrust(FName FromNPCId, FName ToNPCId) const
{
	FDWRelationshipEdge RelationShip;
	if (GetRelationship(FromNPCId, ToNPCId, RelationShip))
	{
		return RelationShip.Trust;
	}
	
	return 0.0f;
}

float UDWRelationshipSubsystem::GetFear(FName FromNPCId, FName ToNPCId) const
{
	FDWRelationshipEdge RelationShip;
	if (!GetRelationship(FromNPCId, ToNPCId, RelationShip))
	{
		return RelationShip.Fear;
	}
	
	return 0.0f;
}

float UDWRelationshipSubsystem::GetRespect(FName FromNPCId, FName ToNPCId) const
{
	FDWRelationshipEdge Relationship;
	if (GetRelationship(FromNPCId, ToNPCId, Relationship))
	{
		return Relationship.Respect;
	}

	return 0.0f;
}

float UDWRelationshipSubsystem::GetResentment(FName FromNPCId, FName ToNPCId) const
{
	FDWRelationshipEdge Relationship;
	if (GetRelationship(FromNPCId, ToNPCId, Relationship))
	{
		return Relationship.Resentment;
	}

	return 0.0f;
}

void UDWRelationshipSubsystem::PrintRelationships() const
{
	if (!GEngine)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.0f,
	FColor::Cyan,
	FString::Printf(TEXT("Relationships: %d"), Relationships.Num())
	);
	
	for (const FDWRelationshipEdge& Relationship : Relationships)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::White,
		FString::Printf(
			TEXT("%s -> %s | Trust %.1f Fear %.1f Respect %.1f Resentment %.1f"),
			*Relationship.FromNPCId.ToString(),
			*Relationship.ToNPCId.ToString(),
			Relationship.Trust,
			Relationship.Fear,
			Relationship.Respect,
			Relationship.Resentment
			)
			);
	}
}

int32 UDWRelationshipSubsystem::FindRelationshipIndex(FName FromNPCId, FName ToNPCId) const
{
	for (int32 Index = 0; Index < Relationships.Num(); Index++)
	{
		const FDWRelationshipEdge& Relationship = Relationships[Index];
		
		if (Relationship.FromNPCId == FromNPCId && Relationship.ToNPCId == ToNPCId)
		{
			return Index;
		}
	}
	
	return INDEX_NONE;
}
