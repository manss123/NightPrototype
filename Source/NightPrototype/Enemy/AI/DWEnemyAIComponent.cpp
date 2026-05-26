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
	
	if (!CurrentTarget)
	{
		AcquirePlayerTarget();
	}
	
	if (!IsTargetValid())
	{
		CurrentTarget = nullptr;
		AIState = EDWEnemmyAIState::Idle;
		return;
	}
	
	if (!ShouldKeepTarget())
	{
		CurrentTarget = nullptr;
		AIState = EDWEnemmyAIState::Idle;
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
	
	AIState = EDWEnemmyAIState::Chasing;
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
	
	AIState = EDWEnemmyAIState::Attacking;
	
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
	if (TargetHealth && TargetHealth->IsIncapacitated())
	{
		return false;
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
	return Distance <= CombatComponent->GetAttackRange() + AttackRangeBuffer;
}

void UDWEnemyAIComponent::StartAI()
{
	if (bIsAIActive || !GetWorld())
	{
		return;
	}
	
	bIsAIActive = true;
	
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
	
	AIState = EDWEnemmyAIState::Idle;
	
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

bool UDWEnemyAIComponent::IsAIActive() const
{
	return bIsAIActive;
}

EDWEnemmyAIState UDWEnemyAIComponent::GetAIState() const
{
	return AIState;
}

