// Fill out your copyright notice in the Description page of Project Settings.


#include "DWGameInstance.h"
#include "Engine/Engine.h"

const FName UDWGameInstance::Event_PlayerInspectedCorpse(TEXT("PlayerInspectedCorpse"));
const FName UDWGameInstance::Event_PlayerReadNoticeBoard(TEXT("PlayerReadNoticeBoard"));

const FName UDWGameInstance::Event_FirstNightStarted(TEXT("NightStarted"));

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
}

void UDWGameInstance::MarkPlayerInspectedCorpse()
{
	AddWorldEvent(Event_PlayerInspectedCorpse);
}

bool UDWGameInstance::HasPlayerInspectedCorpse() const
{
	return HasWorldEvent(Event_PlayerInspectedCorpse);
}

void UDWGameInstance::MarkPlayerReadNoticeBoard()
{
	AddWorldEvent(Event_PlayerReadNoticeBoard);
}

bool UDWGameInstance::HasPlayerReadNoticeBoard() const
{
	return HasWorldEvent(Event_PlayerReadNoticeBoard);
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
	
	if (WorldEventTags.Num() == 0)
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
}
