#include "Combat/Animation/DWCombatAnimationComponent.h"

#include "Animation/AnimInstance.h"
#include "Combat/Core/DWCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Combat/Body/DWBodyHealthComponent.h"


UDWCombatAnimationComponent::UDWCombatAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWCombatAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>())
	{
		CombatComponent->OnAttackStarted.AddDynamic(this, &UDWCombatAnimationComponent::PlayNextAttackMontage);
	}
	
	if (UDWBodyHealthComponent* BodyHealthComponent = Owner->FindComponentByClass<UDWBodyHealthComponent>())
	{
		BodyHealthComponent->OnDamaged.AddDynamic(this, &UDWCombatAnimationComponent::PlayNextHitReactMontage);
	}
}

void UDWCombatAnimationComponent::PlayNextAttackMontage(AActor* TargetActor)
{
	float FinalPlayRate = AttackMontagePlayRate;
	
	if (bUseAttackSpeedForAttackMontage)
	{
		if (const AActor* Owner = GetOwner())
		{
			if (const UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>())
			{
				FinalPlayRate *= CombatComponent->GetAttackSpeedMultiplier();
			}
		}
	}
	PlayMontageFromList(AttackMontages, NextAttackMontageIndex, FinalPlayRate);
}

void UDWCombatAnimationComponent::PlayNextHitReactMontage(FGameplayTag BodyPartTag, float DamageAmount)
{
	PlayMontageFromList(HitReactMontages, NextHitReactMontageIndex, HitReactMontagePlayRate);
}

bool UDWCombatAnimationComponent::PlayMontageFromList(const TArray<TObjectPtr<UAnimMontage>>& Montages, int32& NextIndex, float PlayRate)
{
	if (Montages.Num() == 0)
	{
		return false;
	}
	
	if (!Montages.IsValidIndex(NextIndex))
	{
		NextIndex = 0;
	}
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}
	
	USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComponent)
	{
		return false;
	}
	
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}
	
	UAnimMontage* MontageToPlay = Montages[NextIndex];
	if (!MontageToPlay)
	{
		return false;
	}
	
	AnimInstance->Montage_Play(MontageToPlay, FMath::Max(0.1f, PlayRate));
	
	NextIndex = (NextIndex + 1) % Montages.Num();
	return true;
}
