// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEncounterOutcomeComponent.h"
#include "Combat/Body/DWBodyHealthComponent.h"
#include "DWEncounterDirector.h"
#include "Kismet/GameplayStatics.h"

UDWEncounterOutcomeComponent::UDWEncounterOutcomeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWEncounterOutcomeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
												 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDWEncounterOutcomeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UDWBodyHealthComponent* BodyHealth = Owner->FindComponentByClass<UDWBodyHealthComponent>();
	if (!BodyHealth)
	{
		return;
	}
	
	if (bResolveOnDeath)
	{
		BodyHealth->OnDeath.AddDynamic(this, &UDWEncounterOutcomeComponent::HandleOwnerDeath);
	}
	
	if (bResolveOnDowned)
	{
		BodyHealth->OnDowned.AddDynamic(this, &UDWEncounterOutcomeComponent::HandleOwnerDowned);
	}
}

void UDWEncounterOutcomeComponent::HandleOwnerDeath()
{
	ResolveOutcome(OutcomeOnDeath);
}

void UDWEncounterOutcomeComponent::HandleOwnerDowned()
{
	ResolveOutcome(OutcomeOnDowned);
}

void UDWEncounterOutcomeComponent::ResolveOutcome(FGameplayTag OutcomeTag)
{
	if (EncounterId.IsNone() || !OutcomeTag.IsValid())
	{
		return;
	}
	
	if (bHasResolvedOutcome)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	ADWEncounterDirector* EncounterDirector = Cast<ADWEncounterDirector>(UGameplayStatics::GetActorOfClass(World, ADWEncounterDirector::StaticClass()));
	
	if (!EncounterDirector)
	{
		return;
	}
	
	if (EncounterDirector->ResolveActiveEncounter(EncounterId, OutcomeTag))
	{
		bHasResolvedOutcome = true;
	}
}

