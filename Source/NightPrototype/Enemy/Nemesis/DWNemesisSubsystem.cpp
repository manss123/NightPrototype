// Fill out your copyright notice in the Description page of Project Settings.


#include "DWNemesisSubsystem.h"
#include "Core/DWGameplayTags.h"
#include "Engine/Engine.h"
#include "Enemy/Roster/DWEnemyRosterSubsystem.h"
#include "Enemy/Memory/DWEnemyMemoryComponent.h"

void UDWNemesisSubsystem::SetNemesisState(const FDWNemesisState& State)
{
	if (State.EnemyId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindNemesisStateIndex(State.EnemyId);
	if (ExistingIndex != INDEX_NONE)
	{
		NemesisStates[ExistingIndex] = State;
		return;
	}
	
	NemesisStates.Add(State);
}

bool UDWNemesisSubsystem::GetNemesisState(FName EnemyId, FDWNemesisState& OutState) const
{
	const int32 ExistingIndex = FindNemesisStateIndex(EnemyId);
	if (ExistingIndex == INDEX_NONE)
	{
		return false;
	}
	
	OutState = NemesisStates[ExistingIndex];
	return true;
}

void UDWNemesisSubsystem::MarkEnemyDefeatedPlayer(FName EnemyId)
{
	if (EnemyId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindNemesisStateIndex(EnemyId);
	if (ExistingIndex != INDEX_NONE)
	{
		FDWNemesisState& State = NemesisStates[ExistingIndex];
		State.NemesisTag = DWGameplayTags::Nemesis_Candidate;
		State.EncounterCount++;
		State.bHasDefeatedPlayer = true;
		return;
	}
	
	FDWNemesisState NewState;
	NewState.EnemyId = EnemyId;
	NewState.NemesisTag = DWGameplayTags::Nemesis_Candidate;
	NewState.EncounterCount = 1;
	NewState.bHasDefeatedPlayer = true;
	NemesisStates.Add(NewState);
}

void UDWNemesisSubsystem::MarkEnemyDefeatedByPlayer(FName EnemyId)
{
	if (EnemyId.IsNone())
	{
		return;
	}
	
	const int32 ExistingIndex = FindNemesisStateIndex(EnemyId);
	if (ExistingIndex != INDEX_NONE)
	{
		FDWNemesisState& State = NemesisStates[ExistingIndex];
		State.NemesisTag = DWGameplayTags::Nemesis_Candidate;
		State.EncounterCount++;
		State.bWasDefeatedByPlayer = true;
		return;
	}
	
	FDWNemesisState NewState;
	NewState.EnemyId = EnemyId;
	NewState.NemesisTag = DWGameplayTags::Nemesis_Candidate;
	NewState.EncounterCount = 1;
	NewState.bWasDefeatedByPlayer = true;
	NemesisStates.Add(NewState);
}

void UDWNemesisSubsystem::PrintNemesisStates() const
{
	if (!GEngine)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.0f,
	FColor::Purple,
	FString::Printf(TEXT("Nemesis States: %d"), NemesisStates.Num())
	);
	
	for (const FDWNemesisState& State : NemesisStates)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Purple,
		FString::Printf(
			TEXT("%s | %s | Encounters=%d | DefeatedPlayer=%s | DefeatedByPlayer=%s"),
			*State.EnemyId.ToString(),
			*State.NemesisTag.ToString(),
			State.EncounterCount,
			State.bHasDefeatedPlayer ? TEXT("true") : TEXT("false"),
			State.bWasDefeatedByPlayer ? TEXT("true") : TEXT("false")
			)
		);
	}
}

int32 UDWNemesisSubsystem::FindNemesisStateIndex(FName EnemyId) const
{
	for (int32 Index = 0; Index < NemesisStates.Num(); ++Index)
	{
		if (NemesisStates[Index].EnemyId == EnemyId)
		{
			return Index;
		}
	}
	
	return INDEX_NONE;
}

void UDWNemesisSubsystem::ApplyEnemyMemoryForNemesis(UObject* WorldContextObject, FName EnemyId)
{
	if (!WorldContextObject || EnemyId.IsNone())
	{
		return;
	}
	
	FDWNemesisState State;
	if (!GetNemesisState(EnemyId, State))
	{
		return;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return;
	}
	
	UDWEnemyRosterSubsystem* EnemyRoster = World->GetSubsystem<UDWEnemyRosterSubsystem>();
	if (!EnemyRoster)
	{
		return;
	}
	
	AActor* EnemyActor = EnemyRoster->FindEnemyById(EnemyId);
	if (!IsValid(EnemyActor))
	{
		return;
	}
	
	UDWEnemyMemoryComponent* EnemyMemory = EnemyActor->FindComponentByClass<UDWEnemyMemoryComponent>();
	if (!EnemyMemory)
	{
		return;
	}
	
	if (State.bHasDefeatedPlayer)
	{
		EnemyMemory->AddEnemyMemory(DWGameplayTags::Memory_Enemy_DefeatedPlayer);
	}
	
	if (State.bWasDefeatedByPlayer)
	{
		EnemyMemory->AddEnemyMemory(DWGameplayTags::Memory_Enemy_WasKilledByPlayer);
	}
}
