#include "DWMemoryComponent.h"
#include "Engine/Engine.h"

UDWMemoryComponent::UDWMemoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDWMemoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDWMemoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UDWMemoryComponent::AddMemory(const FDWMemoryEntry& MemoryEntry)
{
	if (!MemoryEntry.MemoryTag.IsValid())
	{
		return;
	}
	
	if (HasMemory(MemoryEntry.MemoryTag))
	{
		return;
	}
	
	FDWMemoryEntry NewMemory = MemoryEntry;
	
	if (!NewMemory.MemoryTag.IsValid())
	{
		NewMemory.MemoryId = FGuid::NewGuid();
	}
	
	Memories.Add(NewMemory);
}

bool UDWMemoryComponent::HasMemory(FGameplayTag MemoryTag) const
{
	if (!MemoryTag.IsValid())
	{
		return false;
	}
	
	for (const FDWMemoryEntry& Memory : Memories)
	{
		if (Memory.MemoryTag.MatchesTagExact(MemoryTag))
		{
			return true;
		}
	}
	
	return false;
}

const TArray<FDWMemoryEntry>& UDWMemoryComponent::GetMemories() const
{
	return Memories;
}

void UDWMemoryComponent::ClearMemories()
{
	Memories.Empty();
}

void UDWMemoryComponent::AddMemoryByTag(FGameplayTag MemoryTag)
{
	if (!MemoryTag.IsValid())
	{
		return;
	}
	
	FDWMemoryEntry MemoryEntry;
	MemoryEntry.MemoryTag = MemoryTag;
	MemoryEntry.Strength = 1.0f;
	
	AddMemory(MemoryEntry);
}

void UDWMemoryComponent::PrintMemories() const
{
	if (!GEngine)
	{
		return;
	}
	
	const AActor* Owner = GetOwner();
	const FString OwnerName = Owner ? Owner->GetName() : TEXT("Unknown");
	
	if (Memories.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		FString::Printf(TEXT("%s Memories: <empty>"), *OwnerName)
		);
		return;
	}
	for (const FDWMemoryEntry& Memory : Memories)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Cyan,
		FString::Printf(
			TEXT("%s Memory: %s | Strength %.2f"),
			*OwnerName,
			*Memory.MemoryTag.ToString(),
			Memory.Strength
			)
		);
	}
}
