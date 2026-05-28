// Fill out your copyright notice in the Description page of Project Settings.


#include "DWBodyHealthComponent.h"
#include "Core/DWGameplayTags.h"
#include "Engine/Engine.h"


UDWBodyHealthComponent::UDWBodyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDWBodyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RegenerateHealth(DeltaTime);
	TryRecoverFromDowned();
}

void UDWBodyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BodyParts.Num() == 0)
	{
		InitializeDefaultBodyParts();
	}
	
	for (FDWBodyPartHealth& BodyPart : BodyParts)
	{
		BodyPart.CurrentHealth = BodyPart.MaxHealth;
		BodyPart.bIsDisabled = false;
	}
	
	bIsDowned = false;
	bIsDead = false;
	BodyState = EDWBodyState::Healthy;
}

void UDWBodyHealthComponent::ApplyDamageToBodyPart(FGameplayTag BodyPartTag, float DamageAmount)
{
	if (IsIncapacitated() || bIsInRecoveryGracePeriod || !BodyPartTag.IsValid() || DamageAmount <= 0.0f)
	{
		return;
	}
	
	FDWBodyPartHealth* BodyPart = FindBodyPart(BodyPartTag);
	if (!BodyPart)
	{
		return;
	}
	
	const float PreviousHealth = BodyPart->CurrentHealth;
	
	BodyPart->CurrentHealth = FMath::Clamp(BodyPart->CurrentHealth - DamageAmount, 0, BodyPart->MaxHealth);
	
	const float ActualDamage = PreviousHealth - BodyPart->CurrentHealth;
	if (ActualDamage <= 0.0f)
	{
		return;
	}
	OnDamaged.Broadcast(BodyPart->BodyPartTag, ActualDamage);
	OnBodyPartDamaged.Broadcast(BodyPart->BodyPartTag, BodyPart->CurrentHealth, BodyPart->MaxHealth);
	
	if (!bIsDead && !bIsDowned && BodyPart->CurrentHealth < BodyPart->MaxHealth)
	{
		BodyState = EDWBodyState::Injured;
	}
	
	if (BodyPart->CurrentHealth <= 0.0f)
	{
		if (!BodyPart->bIsDisabled)
		{
			BodyPart->bIsDisabled = true;
			OnBodyPartDisabled.Broadcast(BodyPart->BodyPartTag);
		}
		if (BodyPart->bDeathOnZero && !bIsDead && !bIsDowned)
		{
			if (bDownedInsteadOfDeathOnVitalZero)
			{
				bIsDowned = true;
				BodyState = EDWBodyState::Downed;
				OnDowned.Broadcast();
			}
			else
			{
				bIsDead = true;
				BodyState = EDWBodyState::Dead;
				OnDeath.Broadcast();
			}
		}
	}
}

void UDWBodyHealthComponent::HealBodyPart(FGameplayTag BodyPartTag, float HealAmount)
{
	if (bIsDead || !BodyPartTag.IsValid() || HealAmount <= 0.0f)
	{
		return;
	}
	
	FDWBodyPartHealth* BodyPart = FindBodyPart(BodyPartTag);
	if (!BodyPart)
	{
		return;
	}
	
	BodyPart->CurrentHealth = FMath::Clamp(BodyPart->CurrentHealth + HealAmount, 0, BodyPart->MaxHealth);
	
	if (BodyPart->CurrentHealth > 0.0f)
	{
		BodyPart->bIsDisabled = false;
	}
	
	OnBodyPartDamaged.Broadcast(BodyPart->BodyPartTag, BodyPart->CurrentHealth, BodyPart->MaxHealth);
}

bool UDWBodyHealthComponent::IsDead() const
{
	return bIsDead;
}

bool UDWBodyHealthComponent::IsBodyPartDisabled(FGameplayTag BodyPartTag) const
{
	const FDWBodyPartHealth* BodyPart = FindBodyPart(BodyPartTag);
	return BodyPart ? BodyPart->bIsDisabled : false;
}

float UDWBodyHealthComponent::GetBodyPartHealthPercent(FGameplayTag BodyPartTag) const
{
	const FDWBodyPartHealth* BodyPart = FindBodyPart(BodyPartTag);
	if (!BodyPart || BodyPart->MaxHealth <= 0.0f)
	{
		return 0.0f;
	}
	
	return BodyPart->CurrentHealth / BodyPart->MaxHealth;
}

void UDWBodyHealthComponent::PrintBodyHealth() const
{
	if (!GEngine)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		bIsDead ? FColor::Red : FColor::Green,
		FString::Printf(
	TEXT("Body Health | State=%s | Dead=%s | Downed=%s"),
			*UEnum::GetValueAsString(BodyState),
			bIsDead ? TEXT("true") : TEXT("false"),
			bIsDowned ? TEXT("true") : TEXT("false")
		)
	);
	
	for (const FDWBodyPartHealth& BodyPart : BodyParts)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			BodyPart.bIsDisabled ? FColor::Orange : FColor::White,
			FString::Printf(
				TEXT("%s: %.1f / %.1f | Disabled=%s | DeathOnZero=%s"),
				*BodyPart.BodyPartTag.ToString(),
				BodyPart.CurrentHealth,
				BodyPart.MaxHealth,
				BodyPart.bIsDisabled ? TEXT("true") : TEXT("false"),
				BodyPart.bDeathOnZero ? TEXT("true") : TEXT("false")
			)
			);
	}
}

FDWBodyPartHealth* UDWBodyHealthComponent::FindBodyPart(FGameplayTag BodyPartTag)
{
	for (FDWBodyPartHealth& BodyPart : BodyParts)
	{
		if (BodyPart.BodyPartTag.MatchesTagExact(BodyPartTag))
		{
			return &BodyPart;
		}
	}
	
	return nullptr;
}

const FDWBodyPartHealth* UDWBodyHealthComponent::FindBodyPart(FGameplayTag BodyPartTag) const
{
	for (const FDWBodyPartHealth& BodyPart : BodyParts)
	{
		if (BodyPart.BodyPartTag.MatchesTagExact(BodyPartTag))
		{
			return &BodyPart;
		}
	}
	
	return nullptr;
}

void UDWBodyHealthComponent::InitializeDefaultBodyParts()
{
	BodyParts = {
		{ DWGameplayTags::Body_Head, FText::FromString(TEXT("Head")), 100.0f, 100.0f, true, false },
		{ DWGameplayTags::Body_Torso, FText::FromString(TEXT("Torso")), 100.0f, 100.0f, true, false },
		{ DWGameplayTags::Body_LeftArm, FText::FromString(TEXT("Left Arm")), 75.0f, 75.0f, false, false },
		{ DWGameplayTags::Body_RightArm, FText::FromString(TEXT("Right Arm")), 75.0f, 75.0f, false, false },
		{ DWGameplayTags::Body_LeftLeg, FText::FromString(TEXT("Left Leg")), 85.0f, 85.0f, false, false },
		{ DWGameplayTags::Body_RightLeg, FText::FromString(TEXT("Right Leg")), 85.0f, 85.0f, false, false }
	};
}

void UDWBodyHealthComponent::ApplyDamageToCore(float DamageAmount)
{
	ApplyDamageToBodyPart(DWGameplayTags::Body_Torso, DamageAmount);
}

void UDWBodyHealthComponent::ApplyDamageToRandomBodyPart(float DamageAmount)
{
	if (IsIncapacitated() || bIsInRecoveryGracePeriod || BodyParts.Num() == 0 || DamageAmount <= 0.0f)
	{
		return;
	}
	
	const int32 RandomIndex = FMath::RandRange(0, BodyParts.Num() - 1);
	ApplyDamageToBodyPart(BodyParts[RandomIndex].BodyPartTag, DamageAmount);
}

bool UDWBodyHealthComponent::IsAnyLegDisabled() const
{
	return IsBodyPartDisabled(DWGameplayTags::Body_LeftLeg) || IsBodyPartDisabled(DWGameplayTags::Body_RightLeg);
}

bool UDWBodyHealthComponent::AreBothLegsDisabled() const
{
	return IsBodyPartDisabled(DWGameplayTags::Body_LeftLeg) && IsBodyPartDisabled(DWGameplayTags::Body_RightLeg);
}

bool UDWBodyHealthComponent::IsAnyArmDisabled() const
{
	return IsBodyPartDisabled(DWGameplayTags::Body_LeftArm) || IsBodyPartDisabled(DWGameplayTags::Body_RightArm);
}

bool UDWBodyHealthComponent::AreBothArmsDisabled() const
{
	return IsBodyPartDisabled(DWGameplayTags::Body_LeftArm) && IsBodyPartDisabled(DWGameplayTags::Body_RightArm);
}

void UDWBodyHealthComponent::SetDowned(bool bNewDowned)
{
	if (bIsDead)
	{
		return;
	}
	
	if (bIsDowned == bNewDowned)
	{
		return;
	}
	
	bIsDowned = bNewDowned;
	
	if (bIsDowned)
	{
		BodyState = EDWBodyState::Downed;
		OnDowned.Broadcast();
	}
	else if (!bIsDead)
	{
		BodyState = EDWBodyState::Injured;
	}
}

bool UDWBodyHealthComponent::IsDowned() const
{
	return bIsDowned;
}

EDWBodyState UDWBodyHealthComponent::GetBodyState() const
{
	return BodyState;
}

bool UDWBodyHealthComponent::IsInjured() const
{
	return BodyState == EDWBodyState::Injured;
}

bool UDWBodyHealthComponent::IsIncapacitated() const
{
	return BodyState == EDWBodyState::Downed || BodyState == EDWBodyState::Unconscious || BodyState == EDWBodyState::Dying || BodyState == EDWBodyState::Dead;
}

void UDWBodyHealthComponent::RegenerateHealth(float DeltaTime)
{
	if (!bCanRegenerate || bIsDead || BodyParts.Num() == 0)
	{
		return;
	}
	
	if (BodyState != EDWBodyState::Injured && 
		BodyState != EDWBodyState::Downed &&
		BodyState != EDWBodyState::Unconscious &&
		BodyState != EDWBodyState::Dying)
	{
		return;
	}
	
	const float RegenAmount = HealthRegenPerSecond * DeltaTime;
	if (RegenAmount <= 0.0f)
	{
		return;
	}
	
	bool bAnyChanged = false;
	
	for (FDWBodyPartHealth& BodyPart : BodyParts)
	{
		if (BodyPart.CurrentHealth >= BodyPart.MaxHealth)
		{
			continue;
		}
		
		BodyPart.CurrentHealth = FMath::Clamp(
			BodyPart.CurrentHealth + RegenAmount,
			0.0f,
			BodyPart.MaxHealth
		);
		
		if (BodyPart.CurrentHealth > 0.0f)
		{
			BodyPart.bIsDisabled = false;
		}
		
		bAnyChanged = true;
	}
	
	if (bAnyChanged)
	{
		for (const FDWBodyPartHealth& BodyPart : BodyParts)
		{
			OnBodyPartDamaged.Broadcast(
				BodyPart.BodyPartTag,
				BodyPart.CurrentHealth,
				BodyPart.MaxHealth
			);
		}
	}
}

void UDWBodyHealthComponent::TryRecoverFromDowned()
{
	if (!bIsDowned || bIsDead)
	{
		return;
	}
	
	const FDWBodyPartHealth* Head = FindBodyPart(DWGameplayTags::Body_Head);
	const FDWBodyPartHealth* Torso = FindBodyPart(DWGameplayTags::Body_Torso);
	
	const bool bHeadRecovered = !Head || Head->CurrentHealth >= Head->MaxHealth * DownedRecoveryHealthPercent;
	const bool bTorsoRecovered = !Torso || Torso->CurrentHealth >= Torso->MaxHealth * DownedRecoveryHealthPercent;
	
	if (!bHeadRecovered || !bTorsoRecovered)
	{
		return;
	}
	
	bIsDowned = false;
	BodyState = EDWBodyState::Injured;
	OnRecovered.Broadcast();
}

void UDWBodyHealthComponent::StartRecoveryGracePeriod(float Duration)
{
	if (Duration <= 0.0f)
	{
		return;
	}
	
	bIsInRecoveryGracePeriod = true;
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RecoveryGraceTimerHandle);
		World->GetTimerManager().SetTimer(
		RecoveryGraceTimerHandle,
		this,
		&UDWBodyHealthComponent::EndRecoveryGracePeriod,
		Duration,
		false
		);
	}
}

void UDWBodyHealthComponent::EndRecoveryGracePeriod()
{
	bIsInRecoveryGracePeriod = false;
}

bool UDWBodyHealthComponent::IsInRecoveryGracePeriod() const
{
	return bIsInRecoveryGracePeriod;
}
