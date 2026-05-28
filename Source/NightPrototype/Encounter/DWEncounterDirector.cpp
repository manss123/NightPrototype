// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEncounterDirector.h"
#include "Core/DWGameInstance.h"
#include "Core/DWGameplayTags.h"
#include "Core/DWWorldEventTypes.h"
#include "Engine/Engine.h"
#include "Enemy/Data/DWEnemyProfileDataAsset.h"
#include "Enemy/Roster/DWEnemyRosterSubsystem.h"
#include "Enemy/Nemesis/DWNemesisSubsystem.h"
#include "World/DWDayNightManager.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/AI/DWEnemyAIComponent.h"
#include "Enemy/Character/DWEnemyCharacter.h"
#include "Combat/Core/DWCombatComponent.h"
#include "Combat/Body/DWBodyHealthComponent.h"

ADWEncounterDirector::ADWEncounterDirector()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADWEncounterDirector::BeginPlay()
{
	Super::BeginPlay();
	
	if (!DayNightManager)
	{
		DayNightManager = Cast<ADWDayNightManager>(
		UGameplayStatics::GetActorOfClass(this, ADWDayNightManager::StaticClass())
		);
	}
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
		const FString VictimActorName = IsValid(Instance.VictimActor) ? Instance.VictimActor->GetName() : TEXT("None");
		
		GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Cyan,
		FString::Printf(
		TEXT("Active Encounter: %s | Event: %s | Location: %s | Threat: %s | EnemyActor= %s | VictimActor= %s | Resolved: %s"),
				*Instance.EncounterId.ToString(),
				*Instance.StartedEventGameplayTag.ToString(),
				*Instance.LocationGameplayTag.ToString(),
				*Instance.ThreatGameplayTag.ToString(),
				*PrimaryEnemyActorName,
				*VictimActorName,
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
		
		if (OutcomeGameplayTag.MatchesTagExact(DWGameplayTags::World_Event_VillagerTaken))
		{
			RevealActors(ActorsToRevealOnVillagerTaken);
		}
		
		ApplyNemesisForOutcome(Instance, OutcomeGameplayTag);
		
		if (UDWGameInstance* DWGameInstance = GetGameInstance<UDWGameInstance>())
		{
			DWGameInstance->AddWorldGameplayTag(OutcomeGameplayTag);
			
			FDWWorldEvent OutcomeEvent;
			OutcomeEvent.EventTag = OutcomeGameplayTag;
			OutcomeEvent.ThreatTag = Instance.EncounterGameplayTag;
			OutcomeEvent.ThreatTag = Instance.ThreatGameplayTag;
			OutcomeEvent.ContextTags = Instance.ContextGameplayTags;
			OutcomeEvent.LocationTag = Instance.LocationGameplayTag;
			OutcomeEvent.SourceActorId = Instance.PrimaryEnemyId;
			
			if (DayNightManager)
			{
				OutcomeEvent.GameDay = DayNightManager->CurrentDay;
				OutcomeEvent.GameHour = DayNightManager->CurrentHour;
			}
			else
			{
				OutcomeEvent.GameDay = Instance.StartGameDay;
				OutcomeEvent.GameHour = Instance.StartGameHour;
			}
			
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
	
	LastKnowGameDay = CurrentDay;
	LastKnownGameHour = CurrentHour;
	
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
	NewInstance.LocationGameplayTag = Template.LocationGameplayTag;
	NewInstance.ThreatGameplayTag = Template.ThreatGameplayTag;
	NewInstance.StartGameDay = CurrentDay;
	NewInstance.StartGameHour = CurrentHour;
	
	AActor* PrimaryEnemyActor = nullptr;
	
	NewInstance.PrimaryEnemyActor = PrimaryEnemyActor;
	
	if (Template.PrimaryEnemyProfile)
	{
		NewInstance.PrimaryEnemyId = Template.PrimaryEnemyProfile->EnemyId;
	}
	
	ActiveEncounters.Add(NewInstance);
	
	 if (bShowDebugMessages)
	 {
	 	PrintActiveEncounters();
	 }
	
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
			StartedEvent.EncounterTag = Template.EncounterGameplayTag;
			StartedEvent.ThreatTag = Template.ThreatGameplayTag;
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
				
				ActivatePrimaryEnemyEncounter(PrimaryEnemyActor);
				
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

void ADWEncounterDirector::ActivatePrimaryEnemyEncounter(AActor* EnemyActor)
{
	if (!IsValid(EnemyActor))
	{
		return;
	}
	
	if (ADWEnemyCharacter* EnemyCharacter = Cast<ADWEnemyCharacter>(EnemyActor))
	{
		EnemyCharacter->SetEnemyRevealed(true);
	}
	
	if (UDWEnemyAIComponent* EnemyAI = EnemyActor->FindComponentByClass<UDWEnemyAIComponent>())
	{
		AActor* TargetActor = GetDefaultEncounterTarget();

		if (TargetActor)
		{
			EnemyAI->SetTarget(TargetActor);
		}
		else
		{
			EnemyAI->StartAI();	
		}
		
		if (ActiveEncounters.Num() > 0)
		{
			ActiveEncounters.Last().VictimActor = TargetActor;
		}
		
		if (bShowDebugMessages)
		{
			PrintActiveEncounters();
		}
		
		BindTargetOutcome(TargetActor);

		if (bShowDebugMessages && GEngine)
		{
			const FString TargetName = IsValid(TargetActor) ? TargetActor->GetName() : TEXT("None");

			GEngine->AddOnScreenDebugMessage(
				-1,
				3.0f,
				FColor::Orange,
				FString::Printf(TEXT("Encounter target: %s"), *TargetName)
			);
		}
	}
	
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Red,
			FString::Printf(TEXT("Encounter activated enemy: %s"), *EnemyActor->GetName())
		);
	}
}

AActor* ADWEncounterDirector::GetDefaultEncounterTarget() const
{
	if (IsValid(DefaultEncounterTargetActor))
	{
		return DefaultEncounterTargetActor;
	}
	
	return UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ADWEncounterDirector::BindTargetOutcome(AActor* TargetActor)
{
	BoundEncounterTargetActor = nullptr;
	
	if (!IsValid(TargetActor))
	{
		return;
	}
	
	UDWBodyHealthComponent* TargetHealth = TargetActor->FindComponentByClass<UDWBodyHealthComponent>();
	if (!TargetHealth)
	{
		return;
	}
	
	BoundEncounterTargetActor = TargetActor;
	
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Purple,
			FString::Printf(TEXT("Bound encounter victim: %s"), *TargetActor->GetName())
		);
	}
	
	TargetHealth->OnDeath.RemoveDynamic(this, &ADWEncounterDirector::HandleEncounterTargetDeath);
	TargetHealth->OnDeath.AddDynamic(this, &ADWEncounterDirector::HandleEncounterTargetDeath);
	
	TargetHealth->OnDowned.RemoveDynamic(this, &ADWEncounterDirector::HandleEncounterTargetDowned);
	TargetHealth->OnDowned.AddDynamic(this, &ADWEncounterDirector::HandleEncounterTargetDowned);
}

void ADWEncounterDirector::HandleEncounterTargetDeath()
{
	ResolveEncountersForBoundTarget(DWGameplayTags::World_Event_VillagerKilled);
}

void ADWEncounterDirector::HandleEncounterTargetDowned()
{
	ResolveEncountersForBoundTarget(DWGameplayTags::World_Event_VillagerTaken);
}

void ADWEncounterDirector::ResolveEncountersForBoundTarget(FGameplayTag OutcomeGameplayTag)
{
	if (!OutcomeGameplayTag.IsValid())
	{
		return;
	}
	
	AActor* TargetActor = BoundEncounterTargetActor.Get();
	if (!IsValid(TargetActor))
	{
		return;
	}
	
	for (const FDWEncounterInstance& Instance : ActiveEncounters)
	{
		if (Instance.bResolved)
		{
			continue;
		}
		
		if (Instance.VictimActor != TargetActor)
		{
			continue;
		}
		
		ResolveActiveEncounter(Instance.EncounterId, OutcomeGameplayTag);
	}
}

void ADWEncounterDirector::RevealActors(const TArray<TObjectPtr<AActor>>& Actors) const
{
	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
	}
}
