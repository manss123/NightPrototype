// Fill out your copyright notice in the Description page of Project Settings.


#include "DWGameInstance.h"
#include "Engine/Engine.h"
#include "DWGameplayTags.h"

const FName UDWGameInstance::Event_PlayerInspectedCorpse(TEXT("PlayerInspectedCorpse"));
const FName UDWGameInstance::Event_PlayerReadNoticeBoard(TEXT("PlayerReadNoticeBoard"));

const FName UDWGameInstance::Event_PlayerSawVampire(TEXT("PlayerSawVampire"));
const FName UDWGameInstance::Encounter_VampireRaid(TEXT("Encounter.VampireRaid"));

const FName UDWGameInstance::Event_PlayerFled(TEXT("World.Event.PlayerFled"));
const FName UDWGameInstance::Event_PlayerDefeated(TEXT("World.Event.PlayerDefeated"));
const FName UDWGameInstance::Event_EnemyKilled(TEXT("World.Event.EnemyKilled"));
const FName UDWGameInstance::Event_EnemyEscaped(TEXT("World.Event.EnemyEscaped"));
const FName UDWGameInstance::Event_VillagerTaken(TEXT("World.Event.VillagerTaken"));
const FName UDWGameInstance::Event_VillagerSaved(TEXT("World.Event.VillagerSaved"));

void UDWGameInstance::AddWorldEvent(FName EventTag)
{
	if (EventTag.IsNone())
	{
		return;
	}
	
	WorldEventTags.Add(EventTag);
}

bool UDWGameInstance::HasWorldEvent(FName EventTag) const
{
	return WorldEventTags.Contains(EventTag);
}

void UDWGameInstance::ClearWorldEvents()
{
	WorldEventTags.Empty();
	WorldEventGameplayTags.Reset();
	WorldEvents.Empty();
}

void UDWGameInstance::MarkPlayerInspectedCorpse()
{
	AddWorldEvent(Event_PlayerInspectedCorpse);
	AddWorldGameplayTag(DWGameplayTags::Memory_Player_InspectedCorpse);
}

bool UDWGameInstance::HasPlayerInspectedCorpse() const
{
	return HasWorldGameplayTag(DWGameplayTags::Memory_Player_InspectedCorpse)
		|| HasWorldEvent(Event_PlayerInspectedCorpse);
}

void UDWGameInstance::MarkPlayerReadNoticeBoard()
{
	AddWorldEvent(Event_PlayerReadNoticeBoard);
	AddWorldGameplayTag(DWGameplayTags::Memory_Player_ReadNoticeBoard);
}

bool UDWGameInstance::HasPlayerReadNoticeBoard() const
{
	return HasWorldGameplayTag(DWGameplayTags::Memory_Player_ReadNoticeBoard) || HasWorldEvent(Event_PlayerReadNoticeBoard);
}

int32 UDWGameInstance::GetWorldEventCount() const
{
	return WorldEventTags.Num();
}

TArray<FName> UDWGameInstance::GetWorldEvents() const
{
	TArray<FName> Events;
	Events.Reserve(WorldEventTags.Num());
	
	for (const FName& EventTag : WorldEventTags)
	{
		Events.Add(EventTag);
	}
	
	return Events;
}

void UDWGameInstance::PrintWorldEvents() const
{
	if (!GEngine)
	{
		return;
	}
	
	if (WorldEventTags.Num() == 0 && WorldEventGameplayTags.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("World Events: <empty>"));
		return;
	}
	
	for (const FName& EventTag : WorldEventTags)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Cyan,
			FString::Printf(TEXT("World Memory: %s"), *EventTag.ToString())
			);
	}
	
	for (const FGameplayTag& EventGameplayTag : WorldEventGameplayTags)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Emerald,
			FString::Printf(TEXT("World GameplayTag: %s"), *EventGameplayTag.ToString())
		);
	}
}

void UDWGameInstance::MarkPlayerSawVampire()
{
	AddWorldEvent(Event_PlayerSawVampire);
	AddWorldGameplayTag(DWGameplayTags::Memory_Player_SawVampire);
}

bool UDWGameInstance::HasPlayerSawVampire() const
{
	return HasWorldGameplayTag(DWGameplayTags::Memory_Player_SawVampire) || HasWorldEvent(Event_PlayerSawVampire);
}

bool UDWGameInstance::HasVampireRaidStarted() const
{
	return HasWorldGameplayTag(DWGameplayTags::World_Event_VampireRaid_Started);
}

void UDWGameInstance::AddWorldGameplayTag(FGameplayTag EventTag)
{
	if (!EventTag.IsValid())
	{
		return;
	}
	
	WorldEventGameplayTags.AddTag(EventTag);
}

bool UDWGameInstance::HasWorldGameplayTag(FGameplayTag EventTag) const
{
	if (!EventTag.IsValid())
	{
		return false;
	}
	
	return WorldEventGameplayTags.HasTagExact(EventTag);
}

void UDWGameInstance::AddWorldEventRecord(const FDWWorldEvent& WorldEvent)
{
	if (!WorldEvent.EventTag.IsValid())
	{
		return;
	}
	
	FDWWorldEvent NewEvent = WorldEvent;
	
	if (!NewEvent.EventId.IsValid())
	{
		NewEvent.EventId = FGuid::NewGuid();
	}
	
	WorldEvents.Add(NewEvent);
	AddWorldGameplayTag(NewEvent.EventTag);
}

const TArray<FDWWorldEvent>& UDWGameInstance::GetWorldEventRecords() const
{
	return WorldEvents;
}

void UDWGameInstance::PrintWorldEventRecords() const
{
	if (!GEngine)
	{
		return;
	}
	
	if (WorldEvents.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Silver,
		TEXT("World Event Records: <empty>")
		);
		return;
	}
	
	for (const FDWWorldEvent& WorldEvent : WorldEvents)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
			5.0f,
			FColor::Silver,
			FString::Printf(
				TEXT("World Event Record: %s | Day %d | Hour %.2f | Location %s"),
				*WorldEvent.EventTag.ToString(),
				WorldEvent.GameDay,
				WorldEvent.GameHour,
				*WorldEvent.LocationTag.ToString()
				)
		);
	}
}

FGameplayTagContainer UDWGameInstance::GetWorldGameplayTags() const
{
	return  WorldEventGameplayTags;
}
