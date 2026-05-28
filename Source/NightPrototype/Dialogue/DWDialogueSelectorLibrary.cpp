// Fill out your copyright notice in the Description page of Project Settings.


#include "DWDialogueSelectorLibrary.h"
#include "Core/DWGameInstance.h"

static bool HasAllTags(const FGameplayTagContainer& OwnedTags, const FGameplayTagContainer& RequiredTags)
{
	if (RequiredTags.IsEmpty())
	{
		return true;
	}
	
	return OwnedTags.HasAllExact(RequiredTags);
}

static bool MemoryHasAllTags(UDWMemoryComponent* MemoryComponent, const FGameplayTagContainer& RequiredMemoryTags)
{
	if (RequiredMemoryTags.IsEmpty())
	{
		return true;
	}
	
	if (!MemoryComponent)
	{
		return false;
	}
	
	for (const FGameplayTag& RequiredTag : RequiredMemoryTags)
	{
		if (!MemoryComponent->HasMemory(RequiredTag))
		{
			return false;
		}
	}
	
	return true;
}

static bool HasEventRecordCondition(const FDWDialogueLine& Line)
{
	return Line.RequiredWorldEventTag.IsValid()
	|| Line.RequiredEncounterTag.IsValid()
	|| Line.RequiredThreatTag.IsValid()
	|| Line.RequiredLocationTag.IsValid();
}

static bool MatchesEventRecordCondition(UDWGameInstance* GameInstance, const FDWDialogueLine& Line)
{
	if (!HasEventRecordCondition(Line))
	{
		return true;
	}
	
	if (!GameInstance)
	{
		return false;
	}
	
	return GameInstance->HasWorldEventRecordMatching(
		Line.RequiredWorldEventTag,
		Line.RequiredEncounterTag,
		Line.RequiredThreatTag,
		Line.RequiredLocationTag
	);
}

bool UDWDialogueSelectorLibrary::SelectBestDialogueLine(UDWDialogueBankDataAsset* DialogueBank, const FGameplayTagContainer& WorldTags, UDWMemoryComponent* MemoryComponent, const FGameplayTagContainer& ContextTags, UDWGameInstance* GameInstance, FDWDialogueLine& OutLine)
{
	if (!DialogueBank)
	{
		return false;
	}
	
	bool bFoundLine = false;
	int32 BestPriority = TNumericLimits<int32>::Min();
	
	for (const FDWDialogueLine& Line : DialogueBank->Lines)
	{
		if (!HasAllTags(WorldTags, Line.RequiredWorldTags))
		{
			continue;
		}
		
		if (!MemoryHasAllTags(MemoryComponent, Line.RequiredMemoryTags))
		{
			continue;
		}
		
		if (!HasAllTags(ContextTags, Line.RequiredContextTags))
		{
			continue;
		}
		
		if (!MatchesEventRecordCondition(GameInstance, Line))
		{
			continue;
		}
		
		if (!bFoundLine || Line.Priority > BestPriority)
		{
			OutLine = Line;
			BestPriority = Line.Priority;
			bFoundLine = true;
		}
	}
	
	return bFoundLine;
}
