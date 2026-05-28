// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyAIComponent.h"
#include "Combat/Core/DWCombatComponent.h"
#include "Combat/Body/DWBodyHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"


UDWEnemyAIComponent::UDWEnemyAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWEnemyAIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (bStartActive)
	{
		StartAI();
	}
}

void UDWEnemyAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopAI();
	Super::EndPlay(EndPlayReason);
}

void UDWEnemyAIComponent::Think()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	const UDWBodyHealthComponent* OwnerHealth = Owner->FindComponentByClass<UDWBodyHealthComponent>();
	if (OwnerHealth && OwnerHealth->IsIncapacitated())
	{
		StopAI();
		return;
	}
	
	if (!CurrentTarget && bAutoAcquirePlayer)
	{
		AcquirePlayerTarget();
	}
	
	if (!IsTargetValid())
	{
		CurrentTarget = nullptr;
		AIState = EDWEnemyAIState::Idle;
		return;
	}
	
	if (!ShouldKeepTarget())
	{
		CurrentTarget = nullptr;
		AIState = EDWEnemyAIState::Idle;
		return;
	}
	
	if (IsTargetInAttackRange())
	{
		AttackTarget();
		return;
	}
	
	ChaseTarget();
}

void UDWEnemyAIComponent::AcquirePlayerTarget()
{
	AActor* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!Player)
	{
		return;
	}
	
	const UDWBodyHealthComponent* PlayerHealth = Player->FindComponentByClass<UDWBodyHealthComponent>();
	if (PlayerHealth && PlayerHealth->IsIncapacitated())
	{
		return;
	}
	
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	const float Distance = FVector::Dist2D(Owner->GetActorLocation(), Player->GetActorLocation());
	if (Distance <= DetectionRange)
	{
		CurrentTarget = Player;
		
		if (bShowDebugMessage && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Red,
			TEXT("Enemy acquired player")
			);
		}
	}
}

void UDWEnemyAIComponent::ChaseTarget()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !CurrentTarget)
	{
		return;
	}
	
	if (UDWCombatComponent* CombatComponent = OwnerCharacter->FindComponentByClass<UDWCombatComponent>())
	{
		if (CombatComponent->IsAutoAttacking())
		{
			CombatComponent->StopAutoAttack();
		}
	}
	
	AController* Controller = OwnerCharacter->GetController();
	if (!Controller)
	{
		return;
	}
	
	AIState = EDWEnemyAIState::Chasing;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, CurrentTarget);
}

void UDWEnemyAIComponent::AttackTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner || !CurrentTarget)
	{
		return;
	}
	
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
	{
		if (AController* Controller = OwnerCharacter->GetController())
		{
			Controller->StopMovement();
		}
	}
	
	AIState = EDWEnemyAIState::Attacking;
	
	if (UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>())
	{
		if (CombatComponent->GetCurrentAttackTarget() != CurrentTarget)
		{
			CombatComponent->StartAutoAttack(CurrentTarget);
		}
	}
}

bool UDWEnemyAIComponent::IsTargetValid() const
{
	if (!CurrentTarget)
	{
		return false;
	}
	
	const UDWBodyHealthComponent* TargetHealth = CurrentTarget->FindComponentByClass<UDWBodyHealthComponent>();
	if (TargetHealth)
	{
		if (TargetHealth->IsIncapacitated())
		{
			return false;
		}
		
		if (TargetHealth->IsInRecoveryGracePeriod())
		{
			return false;
		}
	}
	
	return true;
}

bool UDWEnemyAIComponent::ShouldKeepTarget() const
{
	const AActor* Owner = GetOwner();
	if (!Owner || !CurrentTarget)
	{
		return false;
	}
	
	return FVector::Dist2D(Owner->GetActorLocation(), CurrentTarget->GetActorLocation()) <= LoseTargetRange;
}

bool UDWEnemyAIComponent::IsTargetInAttackRange() const
{
	const AActor* Owner = GetOwner();
	if (!Owner || !CurrentTarget)
	{
		return false;
	}
	
	const UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>();
	if (!CombatComponent)
	{
		return false;
	}
	
	const float Distance = FVector::Dist2D(Owner->GetActorLocation(), CurrentTarget->GetActorLocation());
	return Distance <= CombatComponent->GetAttackRange();
}

void UDWEnemyAIComponent::StartAI()
{
	if (bIsAIActive || !GetWorld())
	{
		return;
	}
	
	bIsAIActive = true;
	
	Think();
	
	GetWorld()->GetTimerManager().SetTimer(
	ThinkTimerHandle,
	this,
	&UDWEnemyAIComponent::Think,
	ThinkInterval,
	true
	);
}

void UDWEnemyAIComponent::StopAI()
{
	if (!bIsAIActive)
	{
		return;
	}
	
	bIsAIActive = false;
	CurrentTarget = nullptr;
	
	AIState = EDWEnemyAIState::Idle;
	
	if (AActor* Owner = GetOwner())
	{
		if (UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>())
		{
			CombatComponent->StopAutoAttack();
		}
	}
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ThinkTimerHandle);
	}
}

void UDWEnemyAIComponent::SetTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	
	if (!IsTargetValid())
	{
		CurrentTarget = nullptr;
		AIState = EDWEnemyAIState::Idle;
		return;
	}
	
	if (!bIsAIActive)
	{
		StartAI();
		return;
	}
	
	Think();
}

void UDWEnemyAIComponent::ClearTarget()
{
	CurrentTarget = nullptr;
	AIState = EDWEnemyAIState::Idle;
	
	if (AActor* Owner = GetOwner())
	{
		if (UDWCombatComponent* CombatComponent = Owner->FindComponentByClass<UDWCombatComponent>())
		{
			CombatComponent->StopAutoAttack();
		}
	}
}

bool UDWEnemyAIComponent::IsAIActive() const
{
	return bIsAIActive;
}

EDWEnemyAIState UDWEnemyAIComponent::GetAIState() const
{
	return AIState;
}

AActor* UDWEnemyAIComponent::GetCurrentTarget() const
{
	return CurrentTarget;
}
