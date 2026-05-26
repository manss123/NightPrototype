// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEncounterDirector.h"
#include "Core/DWGameInstance.h"
#include "Core/DWGameplayTags.h"
#include "Core/DWWorldEventTypes.h"
#include "Engine/Engine.h"
#include "Enemy/Data/DWEnemyProfileDataAsset.h"
#include "Enemy/Roster/DWEnemyRosterSubsystem.h"
#include "Enemy/Nemesis/DWNemesisSubsystem.h"

// Sets default values
ADWEncounterDirector::ADWEncounterDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADWEncounterDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ADWEncounterDirector::TryStartEncounter(UDWEncounterTemplateDataAsset* EncounterData)
{
	return StartEncounterInternal(EncounterData, 1, 0.0f);
}

bool ADWEncounterDirector::TryStartEncounterWithTime(UDWEncounterTemplateDataAsset* EncounterData, int32 CurrentDay, float CurrentHour)
{
	return StartEncounterInternal(EncounterData, CurrentDay, CurrentHour);
}

bool ADWEncounterDirector::CanStartEncounter(UDWEncounterTemplateDataAsset* EncounterData) const
{
	if (!EncounterData)
	{
		return false;
	}
	
	const FDWEncounterTemplate& Template = EncounterData->EncounterTemplate;
	
	if (Template.EncounterId.IsNone())
	{
		return false;
	}
	
	const UDWGameInstance* DWGameInstance = GetGameInstance<UDWGameInstance>();
	
	if (!DWGameInstance)
	{
		return false;
	}
	
	if (Template.bOneShot && Template.EncounterGameplayTag.IsValid())
	{
		if (DWGameInstance->HasWorldGameplayTag(Template.EncounterGameplayTag))
		{
			return false;
		}
	}
	
	for (const FGameplayTag& RequiredGameplayTag : Template.RequiredWorldGameplayTags)
	{
		if (RequiredGameplayTag.IsValid() && !DWGameInstance->HasWorldGameplayTag(RequiredGameplayTag))
		{
			return false;
		}
	}
	
	for (const FGameplayTag& BlockedGameplayTag : Template.BlockedWorldGameplayTags)
	{
		if (BlockedGameplayTag.IsValid() && DWGameInstance->HasWorldGameplayTag(BlockedGameplayTag))
		{
			return false;
		}
	}
	
	return true;
}

void ADWEncounterDirector::PrintActiveEncounters() const
{
	if (!GEngine)
	{
		return;
	}
	
	if (ActiveEncounters.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("Active Encounters: <none>")
		);
		return;
	}
	
	for (const FDWEncounterInstance& Instance : ActiveEncounters)
	{
		const FString PrimaryEnemyActorName = IsValid(Instance.PrimaryEnemyActor) ? Instance.PrimaryEnemyActor->GetName() : TEXT("None");
		GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Cyan,
		FString::Printf(
			TEXT("Active Encounter: %s | Event: %s | EnemyActor= %s | Resolved: %s"),
			*Instance.EncounterId.ToString(),
			*Instance.StartedEventGameplayTag.ToString(),
			*PrimaryEnemyActorName,
			Instance.bResolved ? TEXT("true") : TEXT("false")
			)
		);
	}
}

bool ADWEncounterDirector::ResolveActiveEncounter(FName EncounterId, FGameplayTag OutcomeGameplayTag)
{
	if (EncounterId.IsNone() || !OutcomeGameplayTag.IsValid())
	{
		return false;
	}
	
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			4.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Resolve requested: %s"), *EncounterId.ToString())
		);
	}
	
	for (FDWEncounterInstance& Instance : ActiveEncounters)
	{
		if (Instance.EncounterId != EncounterId || Instance.bResolved)
		{
			continue;
		}
		
		Instance.OutcomeGameplayTag = OutcomeGameplayTag;
		Instance.bResolved = true;
		
		ApplyNemesisForOutcome(Instance, OutcomeGameplayTag);
		
		if (UDWGameInstance* DWGameInstance = GetGameInstance<UDWGameInstance>())
		{
			DWGameInstance->AddWorldGameplayTag(OutcomeGameplayTag);
			
			FDWWorldEvent OutcomeEvent;
			OutcomeEvent.EventTag = OutcomeGameplayTag;
			OutcomeEvent.ContextTags = Instance.ContextGameplayTags;
			OutcomeEvent.GameDay = 1;
			OutcomeEvent.GameHour = 0.0f;
			
			DWGameInstance->AddWorldEventRecord(OutcomeEvent);
			
			if (bPrintWorldEventsOnStart)
			{
				DWGameInstance->PrintWorldEvents();
				DWGameInstance->PrintWorldEventRecords();
			}
		}
		
		if (bShowDebugMessages && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			4.0f,
			FColor::Green,
			FString::Printf(
				TEXT("Encounter resolved: %s -> %s"),
				*EncounterId.ToString(),
				*OutcomeGameplayTag.ToString()
				)
			);
		}
		
		return  true;
	}
	
	return  false;
}

UDWEncounterTemplateDataAsset* ADWEncounterDirector::FindBestEligibleEncountersByGameplayContext(const FGameplayTagContainer& ContextTags) const
{
	UDWEncounterTemplateDataAsset* BestEncounter = nullptr;
	int32 BestPriority = TNumericLimits<int32>::Min();
	
	for (UDWEncounterTemplateDataAsset* EncounterData : EncounterTemplates)
	{
		if (!EncounterData || !CanStartEncounter(EncounterData))
		{
			continue;
		}
		
		const FDWEncounterTemplate& Template = EncounterData->EncounterTemplate;
		
		if (!ContextTags.IsEmpty())
		{
			if (Template.ContextGameplayTags.IsEmpty())
			{
				continue;
			}
			
			if (!ContextTags.HasAnyExact(Template.ContextGameplayTags))
			{
				continue;
			}
		}
		
		if (!BestEncounter || Template.Priority > BestPriority)
		{
			BestEncounter = EncounterData;
			BestPriority = Template.Priority;
		}
	}
	
	return BestEncounter;
}

bool ADWEncounterDirector::EvaluateEncountersWithGameplayContext(const FGameplayTagContainer& ContextTags)
{
	UDWEncounterTemplateDataAsset* BestEncounter = FindBestEligibleEncountersByGameplayContext(ContextTags);
	
	if (!BestEncounter)
	{
		if (bShowDebugMessages && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Orange,
			TEXT("No eligible encounter found for gameplay context")
			);
		}
		
		return false;
	}
	
	return TryStartEncounter(BestEncounter);
}

bool ADWEncounterDirector::EvaluateEncountersWithGameplayContextAndTime(const FGameplayTagContainer& ContextTags, int32 CurrentDay, float CurrentHour)
{
	UDWEncounterTemplateDataAsset* BestEncounter = FindBestEligibleEncountersByGameplayContext(ContextTags);
	
	if (!BestEncounter)
	{
		if (bShowDebugMessages && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Orange,
			TEXT("No eligible encounter found for gameplay context")
			);
		}
		
		return false;
	}
	
	return TryStartEncounterWithTime(BestEncounter, CurrentDay, CurrentHour);
}

bool ADWEncounterDirector::StartEncounterInternal(UDWEncounterTemplateDataAsset* EncounterData, int32 CurrentDay, float CurrentHour)
{
	if (!EncounterData)
	{
		return false;
	}
	
	if (!CanStartEncounter(EncounterData))
	{
		return false;
	}
	
	const FDWEncounterTemplate& Template = EncounterData->EncounterTemplate;
	
	if (Template.EncounterId.IsNone())
	{
		return false;
	}
	
	FDWEncounterInstance NewInstance;
	NewInstance.InstanceId = FGuid::NewGuid();
	NewInstance.EncounterId = Template.EncounterId;
	NewInstance.EncounterGameplayTag = Template.EncounterGameplayTag;
	NewInstance.StartedEventGameplayTag = Template.StartedEventGameplayTag;
	NewInstance.ContextGameplayTags = Template.ContextGameplayTags;
	
	AActor* PrimaryEnemyActor = nullptr;
	
	NewInstance.PrimaryEnemyActor = PrimaryEnemyActor;
	
	if (Template.PrimaryEnemyProfile)
	{
		NewInstance.PrimaryEnemyId = Template.PrimaryEnemyProfile->EnemyId;
	}
	
	ActiveEncounters.Add(NewInstance);
	
	// if (bShowDebugMessages)
	// {
	// 	PrintActiveEncounters();
	// }
	
	if (UDWGameInstance* DWGameInstance = GetGameInstance<UDWGameInstance>())
	{
		if (Template.EncounterGameplayTag.IsValid())
		{
			DWGameInstance->AddWorldGameplayTag(Template.EncounterGameplayTag);
		}

		if (Template.StartedEventGameplayTag.IsValid())
		{
			DWGameInstance->AddWorldGameplayTag(Template.StartedEventGameplayTag);
		}

		if (Template.StartedEventGameplayTag.IsValid())
		{
			FDWWorldEvent StartedEvent;
			StartedEvent.EventTag = Template.StartedEventGameplayTag;
			StartedEvent.ContextTags = Template.ContextGameplayTags;
			StartedEvent.LocationTag = Template.LocationGameplayTag;
			StartedEvent.GameHour = CurrentHour;
			StartedEvent.GameDay = CurrentDay;
			
			DWGameInstance->AddWorldEventRecord(StartedEvent);
		}
		
		if (bPrintWorldEventsOnStart)
		{
			DWGameInstance->PrintWorldEvents();
			DWGameInstance->PrintWorldEventRecords();
		}
	}
	
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Red,
		FString::Printf(TEXT("Encounter started: %s"), *Template.EncounterId.ToString())
		);
	}
	
	if (bShowDebugMessages && GEngine && Template.PrimaryEnemyProfile)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			4.0f,
			FColor::Red,
			FString::Printf(
				TEXT("Primary enemy: %s (%s)"),
				*Template.PrimaryEnemyProfile->DisplayName.ToString(),
				*Template.PrimaryEnemyProfile->EnemyId.ToString()
			)
		);
	}
	
	if (Template.PrimaryEnemyProfile)
	{
		if (UWorld* World = GetWorld())
		{
			if (UDWEnemyRosterSubsystem* EnemyRoster = World->GetSubsystem<UDWEnemyRosterSubsystem>())
			{
				PrimaryEnemyActor = EnemyRoster->FindEnemyById(Template.PrimaryEnemyProfile->EnemyId);
				
				if (ActiveEncounters.Num() > 0)
				{
					ActiveEncounters.Last().PrimaryEnemyActor = PrimaryEnemyActor;
				}
				
				if (bShowDebugMessages && GEngine)
				{
					const FString EnemyActorName = IsValid(PrimaryEnemyActor) ? PrimaryEnemyActor->GetName() : TEXT("None");
					
					GEngine->AddOnScreenDebugMessage(
					-1,
					4.0f,
					IsValid(PrimaryEnemyActor) ? FColor::Green : FColor::Orange,
					FString::Printf(TEXT("Primary enemy actor: %s"), *EnemyActorName)
					);
				}
			}
		}
	}
	
	return true;
}

void ADWEncounterDirector::ApplyNemesisForOutcome(const FDWEncounterInstance& Instance, FGameplayTag OutcomeGameplayTag)
{
	if (!OutcomeGameplayTag.IsValid() || Instance.PrimaryEnemyId.IsNone())
	{
		return;
	}
	
	if (!IsValid(Instance.PrimaryEnemyActor))
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return;
	}
	
	UDWNemesisSubsystem* NemesisSubsystem = GameInstance->GetSubsystem<UDWNemesisSubsystem>();
	if (!NemesisSubsystem)
	{
		return;
	}
	
	const FName EnemyId = Instance.PrimaryEnemyId;
	
	if (OutcomeGameplayTag.MatchesTagExact(DWGameplayTags::World_Event_PlayerDefeated))
	{
		NemesisSubsystem->MarkEnemyDefeatedByPlayer(EnemyId);
		NemesisSubsystem->ApplyEnemyMemoryForNemesis(this, EnemyId);
		return;
	}
	
	if (OutcomeGameplayTag.MatchesTagExact(DWGameplayTags::World_Event_EnemyKilled))
	{
		NemesisSubsystem->MarkEnemyDefeatedByPlayer(EnemyId);
		NemesisSubsystem->ApplyEnemyMemoryForNemesis(this, EnemyId);
		return;
	}
}
