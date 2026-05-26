// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyMemoryComponent.h"
#include "Engine/Engine.h"


UDWEnemyMemoryComponent::UDWEnemyMemoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWEnemyMemoryComponent::AddEnemyMemory(FGameplayTag MemoryTag)
{
	if (!MemoryTag.IsValid())
	{
		return;
	}
	
	EnemyMemoryTags.AddTag(MemoryTag);
}

bool UDWEnemyMemoryComponent::HasEnemyMemory(FGameplayTag MemoryTag) const
{
	if (!MemoryTag.IsValid())
	{
		return false;
	}
	
	return EnemyMemoryTags.HasTagExact(MemoryTag);
}

void UDWEnemyMemoryComponent::PrintEnemyMemories() const
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Purple,
		FString::Printf(TEXT("Enemy Memories: %d"), EnemyMemoryTags.Num())
	);

	TArray<FGameplayTag> Tags;
	EnemyMemoryTags.GetGameplayTagArray(Tags);

	for (const FGameplayTag& Tag : Tags)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Purple,
			Tag.ToString()
		);
	}
}

