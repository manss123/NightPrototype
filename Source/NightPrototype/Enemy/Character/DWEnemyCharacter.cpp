// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyCharacter.h"
#include "Enemy/Data/DWEnemyProfileDataAsset.h"
#include "Enemy/Roster/DWEnemyRosterSubsystem.h"
#include "Combat/Body/DWBodyHealthComponent.h"
#include "Combat/Core/DWCombatComponent.h"
#include "Enemy/AI/DWEnemyAIComponent.h"


ADWEnemyCharacter::ADWEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InteractText = FText::FromString(TEXT("Attack"));
	InteractDistance = 150.0f;
	bCanInteract = true;
	bShouldFaceInteractor = true;
	bUseDynamicInteractLocation = true;
	DynamicInteractDistance = 150.0f;
}

void ADWEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADWEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADWEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterWithEnemyRoster();
	
	SetEnemyRevealed(bStartRevealed);
}

void ADWEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromEnemyRoster();
	
	Super::EndPlay(EndPlayReason);
}

UDWEnemyProfileDataAsset* ADWEnemyCharacter::GetEnemyProfile_Implementation() const
{
	return EnemyProfile;
}

void ADWEnemyCharacter::InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action)
{
	if (Action == EDWInteractionAction::Attack)
	{
		HandleAttackInteraction(Interactor);
		return;
	}
	
	if (Action == EDWInteractionAction::Inspect)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Cyan,
			FString::Printf(TEXT("Inspecting corpse: %s"), *GetName())
			);
		}
		return;
	}
	
	Super::InteractWithOption_Implementation(Interactor, Action);
}

bool ADWEnemyCharacter::CanInteract_Implementation(AActor* Interactor)
{
	if (!bIsRevealed)
	{
		return false;
	}
	
	return Super::CanInteract_Implementation(Interactor);
}

void ADWEnemyCharacter::RegisterWithEnemyRoster()
{
	if (UWorld* World = GetWorld())
	{
		if (UDWEnemyRosterSubsystem* EnemyRoster = World->GetSubsystem<UDWEnemyRosterSubsystem>())
		{
			EnemyRoster->RegisterEnemy(this);
		}
	}
}

void ADWEnemyCharacter::UnregisterFromEnemyRoster()
{
	if (UWorld* World = GetWorld())
	{
		if (UDWEnemyRosterSubsystem* EnemyRoster = World->GetSubsystem<UDWEnemyRosterSubsystem>())
		{
			EnemyRoster->UnregisterEnemy(this);
		}
	}
}

void ADWEnemyCharacter::HandleAttackInteraction(AActor* Interactor)
{
	if (Interactor)
	{
		if (UDWCombatComponent* CombatComponent = Interactor->FindComponentByClass<UDWCombatComponent>())
		{
			CombatComponent->StartAutoAttack(this);
			return;
		}
	}
	
	UDWBodyHealthComponent* BodyHealth = FindComponentByClass<UDWBodyHealthComponent>();
	if (!BodyHealth)
	{
		return;
	}
	
	BodyHealth->ApplyDamageToCore(AttackDamageReceived);
	
	if (bPrintBodyHealthAfterAttack)
	{
		BodyHealth->PrintBodyHealth();
	}
}


TArray<FDWInteractionOption> ADWEnemyCharacter::GetInteractionOptions_Implementation()
{
	TArray<FDWInteractionOption> Options;
	
	if (IsEnemyIncapacitated())
	{
		FDWInteractionOption InspectOption;
		InspectOption.Action = EDWInteractionAction::Inspect;
		InspectOption.Label = FText::FromString(TEXT("Inspect"));
		Options.Add(InspectOption);
		return Options;
	}
	
	FDWInteractionOption AttackOption;
	AttackOption.Action = EDWInteractionAction::Attack;
	AttackOption.Label = FText::FromString(TEXT("Attack"));
	Options.Add(AttackOption);
	return Options;
}

FText ADWEnemyCharacter::GetInteractText_Implementation()
{
	return IsEnemyIncapacitated() ? FText::FromString(TEXT("Inspect")) : FText::FromString(TEXT("Attack"));
}

bool ADWEnemyCharacter::IsEnemyDead() const
{
	const UDWBodyHealthComponent* BodyHealth = FindComponentByClass<UDWBodyHealthComponent>();
	return BodyHealth && BodyHealth->IsDead();
}

bool ADWEnemyCharacter::IsEnemyIncapacitated() const
{
	const UDWBodyHealthComponent* BodyHealth = FindComponentByClass<UDWBodyHealthComponent>();
	return BodyHealth && BodyHealth->IsIncapacitated();
}

void ADWEnemyCharacter::SetEnemyRevealed(bool bRevealed)
{
	bIsRevealed = bRevealed;
	
	if (!bIsRevealed)
	{
		if (UDWEnemyAIComponent* EnemyAI = FindComponentByClass<UDWEnemyAIComponent>())
		{
			EnemyAI->StopAI();
		}
		
		if (UDWCombatComponent* CombatComponent = FindComponentByClass<UDWCombatComponent>())
		{
			CombatComponent->StopAutoAttack();
		}
		
		if (AController* EnemyController = GetController())
		{
			EnemyController->StopMovement();
		}
	}
	
	SetActorHiddenInGame(!bIsRevealed);
	SetActorEnableCollision(bIsRevealed);
	SetActorTickEnabled(bIsRevealed);
}

bool ADWEnemyCharacter::IsEnemyRevealed() const
{
	return bIsRevealed;
}
