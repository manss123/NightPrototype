// Fill out your copyright notice in the Description page of Project Settings.


#include "DWDialogueSelectorLibrary.h"

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

bool UDWDialogueSelectorLibrary::SelectBestDialogueLine(UDWDialogueBankDataAsset* DialogueBank, const FGameplayTagContainer& WorldTags, UDWMemoryComponent* MemoryComponent, const FGameplayTagContainer& ContextTags, FDWDialogueLine& OutLine)
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
		
		if (!bFoundLine || Line.Priority > BestPriority)
		{
			OutLine = Line;
			BestPriority = Line.Priority;
			bFoundLine = true;
		}
	}
	
	return bFoundLine;
}
