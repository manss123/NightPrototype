// Fill out your copyright notice in the Description page of Project Settings.


#include "DWNPCRegistrySubsystem.h"
#include "DWNPCProfileProvider.h"
#include "DWNPCProfileDataAsset.h"
#include "Engine/Engine.h"

void UDWNPCRegistrySubsystem::RegisterNPC(AActor* NPCActor)
{
	if (!IsValid(NPCActor))
	{
		return;
	}
	
	if (!NPCActor->GetClass()->ImplementsInterface(UDWNPCProfileProvider::StaticClass()))
	{
		return;
	}
	
	RegisteredNPCs.AddUnique(NPCActor);
}

void UDWNPCRegistrySubsystem::UnregisterNPC(AActor* NPCActor)
{
	RegisteredNPCs.Remove(NPCActor);
}

TArray<AActor*> UDWNPCRegistrySubsystem::GetRegisteredNPCs() const
{
	TArray<AActor*> Result;

	for (AActor* NPCActor : RegisteredNPCs)
	{
		if (IsValid(NPCActor))
		{
			Result.Add(NPCActor);
		}
	}
	
	return Result;
}

AActor* UDWNPCRegistrySubsystem::FindNPCById(FName NPCId) const
{
	for (AActor* NPCActor : RegisteredNPCs)
	{
		if (!IsValid(NPCActor))
		{
			continue;
		}
		
		if (!NPCActor->GetClass()->ImplementsInterface(UDWNPCProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWNPCProfileDataAsset* Profile = IDWNPCProfileProvider::Execute_GetNPCProfile(NPCActor);
		if (Profile && Profile->NPCId == NPCId)
		{
			return NPCActor;
		}
	}
	
	return nullptr;
}

void UDWNPCRegistrySubsystem::PrintRegisteredNPCs() const
{
	if (!GEngine)
	{
		return;
	}
	
	int32 ValidCount = 0;
	
	for (AActor* NPCActor : RegisteredNPCs)
	{
		if (!IsValid(NPCActor))
		{
			continue;
		}
		
		ValidCount++;
		
		FString NPCIdText = TEXT("NoProfile");
		
		if (NPCActor->GetClass()->ImplementsInterface(UDWNPCProfileProvider::StaticClass()))
		{
			if (UDWNPCProfileDataAsset* Profile = IDWNPCProfileProvider::Execute_GetNPCProfile(NPCActor))
			{
				NPCIdText = Profile->NPCId.ToString();
			}
		}
		
		GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Cyan,
		FString::Printf(TEXT("Registered NPC: %s (%s)"), *NPCActor->GetName(), *NPCIdText)
		);
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.0f,
	FColor::Green,
	FString::Printf(TEXT("NPC Registry Count: %d"), ValidCount)
	);
}

TArray<AActor*> UDWNPCRegistrySubsystem::FindNPCsByRole(FGameplayTag RoleTag) const
{
	TArray<AActor*> Result;
	
	if (!RoleTag.IsValid())
	{
		return Result;
	}
	
	for (AActor*  NPCActor : RegisteredNPCs)
	{
		if (!IsValid(NPCActor))
		{
			continue;
		}
		
		if (!NPCActor->GetClass()->ImplementsInterface(UDWNPCProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWNPCProfileDataAsset* Profile = IDWNPCProfileProvider::Execute_GetNPCProfile(NPCActor);
		if (!Profile)
		{
			continue;
		}
		
		if (Profile->RoleTag.MatchesTagExact(RoleTag))
		{
			Result.Add(NPCActor);
		}
	}
	
	return Result;
}

TArray<AActor*> UDWNPCRegistrySubsystem::FindNPCsByLocation(FGameplayTag LocationTag) const
{
	TArray<AActor*> Result;
	
	if (!LocationTag.IsValid())
	{
		return Result;
	}
	
	for (AActor* NPCActor : RegisteredNPCs)
	{
		if (!IsValid(NPCActor))
		{
			continue;
		}
		
		if (!NPCActor->GetClass()->ImplementsInterface(UDWNPCProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWNPCProfileDataAsset* Profile = IDWNPCProfileProvider::Execute_GetNPCProfile(NPCActor);
		if (!Profile)
		{
			continue;
		}
		
		if (Profile->LocationTag.MatchesTagExact(LocationTag))
		{
			Result.Add(NPCActor);
		}
	}
	
	return Result;
}
