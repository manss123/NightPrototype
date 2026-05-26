// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyRosterSubsystem.h"
#include "Enemy/Data/DWEnemyProfileProvider.h"
#include "Enemy/Data/DWEnemyProfileDataAsset.h"
#include "Engine/Engine.h"

void UDWEnemyRosterSubsystem::RegisterEnemy(AActor* EnemyActor)
{
	if (!IsValid(EnemyActor))
	{
		return;
	}
	
	if (!EnemyActor->GetClass()->ImplementsInterface(UDWEnemyProfileProvider::StaticClass()))
	{
		return;
	}
	
	RegisteredEnemies.AddUnique(EnemyActor);
}

void UDWEnemyRosterSubsystem::UnregisterEnemy(AActor* EnemyActor)
{
	RegisteredEnemies.Remove(EnemyActor);
}

TArray<AActor*> UDWEnemyRosterSubsystem::GetRegisteredEnemies() const
{
	TArray<AActor*> Result;
	
	for (AActor* EnemyActor : RegisteredEnemies)
	{
		if (IsValid(EnemyActor))
		{
			Result.Add(EnemyActor);
		}
	}
	
	return Result;
}

AActor* UDWEnemyRosterSubsystem::FindEnemyById(FName EnemyId) const
{
	for (AActor* EnemyActor : RegisteredEnemies)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}
		
		if (!EnemyActor->GetClass()->ImplementsInterface(UDWEnemyProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWEnemyProfileDataAsset* Profile = IDWEnemyProfileProvider::Execute_GetEnemyProfile(EnemyActor);
		if (Profile && Profile->EnemyId == EnemyId)
		{
			return EnemyActor;
		}
	}
	
	return nullptr;
}

TArray<AActor*> UDWEnemyRosterSubsystem::FindEnemyByThreat(FGameplayTag ThreatTag) const
{
	TArray<AActor*> Result;
	
	if (!ThreatTag.IsValid())
	{
		return Result;
	}
	
	for (AActor* EnemyActor : RegisteredEnemies)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}
		
		if (!EnemyActor->GetClass()->ImplementsInterface(UDWEnemyProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWEnemyProfileDataAsset* Profile = IDWEnemyProfileProvider::Execute_GetEnemyProfile(EnemyActor);
		if (Profile && Profile->ThreatTag == ThreatTag)
		{
			Result.Add(EnemyActor);
		}
	}
	
	return Result;
}

TArray<AActor*> UDWEnemyRosterSubsystem::FindEnemyByType(FGameplayTag EnemyTypeTag) const
{
	TArray<AActor*> Result;
	
	if (!EnemyTypeTag.IsValid())
	{
		return Result;
	}
	
	for (AActor* EnemyActor : RegisteredEnemies)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}
		
		if (!EnemyActor->GetClass()->ImplementsInterface(UDWEnemyProfileProvider::StaticClass()))
		{
			continue;
		}
		
		UDWEnemyProfileDataAsset* Profile = IDWEnemyProfileProvider::Execute_GetEnemyProfile(EnemyActor);
		if (Profile && Profile->EnemyTypeTag.MatchesTagExact(EnemyTypeTag))
		{
			Result.Add(EnemyActor);
		}
	}
	
	return Result;
}

void UDWEnemyRosterSubsystem::PrintRegisteredEnemies() const
{
	if (!GEngine)
	{
		return;
	}
	
	int32 ValidCount = 0;
	
	for (AActor* EnemyActor : RegisteredEnemies)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}
		
		ValidCount++;
		
		FString EnemyIdText = TEXT("NoProfile");
		FString ThreatText = TEXT("NoThreat");
		FString EnemyTypeText = TEXT("NoEnemyType");
		
		if (EnemyActor->GetClass()->ImplementsInterface(UDWEnemyProfileProvider::StaticClass()))
		{
			if (UDWEnemyProfileDataAsset* Profile = IDWEnemyProfileProvider::Execute_GetEnemyProfile(EnemyActor))
			{
				EnemyIdText = Profile->EnemyId.ToString();
				ThreatText = Profile->ThreatTag.ToString();
				EnemyTypeText = Profile->EnemyTypeTag.ToString();
			}
		}
		
		GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Red,
		FString::Printf(
	TEXT("Registered Enemy: %s (%s) Threat=%s Type=%s"),
				*EnemyActor->GetName(),
				*EnemyIdText,
				*ThreatText,
				*EnemyTypeText
			)
		);
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.0f,
	FColor::Orange,
	FString::Printf(TEXT("Enemy Roster Count: %d"), ValidCount)
	);
}
