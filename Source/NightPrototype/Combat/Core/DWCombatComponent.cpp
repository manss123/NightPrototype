// Fill out your copyright notice in the Description page of Project Settings.


#include "DWCombatComponent.h"
#include "Combat/Body/DWBodyHealthComponent.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UDWCombatComponent::UDWCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWCombatComponent::AttackActor(AActor* TargetActor)
{
	if (!CanTargetActor(TargetActor))
	{
		if (bShowDebugCombatMessages && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Orange,
			TEXT("Attack failed")
			);
		}
		return;
	}

	UDWBodyHealthComponent* TargetHealth = TargetActor->FindComponentByClass<UDWBodyHealthComponent>();
	if (!TargetHealth)
	{
		return;
	}
	
	if (bFaceTargetOnAttack)
	{
		FaceTarget(TargetActor);
	}
	
	const float DamageAmount = GetAttackDamage();
	TargetHealth->ApplyDamageToCore(DamageAmount);
	
	OnAttackHit.Broadcast(TargetActor, DamageAmount);
	
	if (bShowDebugCombatMessages && GEngine)
	{
		const FString OwnerName = GetOwner() ? GetOwner()->GetName() : TEXT("Unknown");
		const FString TargetName =  TargetActor ? TargetActor->GetName() : TEXT("Unknown");
		
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Red,
			FString::Printf(
				TEXT("%s attacked %s: %.1f damage"),
				*OwnerName,
				*TargetName,
				DamageAmount
			)
		);
	}
	
	if (bPrintTargetHealthAfterAttack)
	{
		TargetHealth->PrintBodyHealth();
	}
}

void UDWCombatComponent::StartAutoAttack(AActor* TargetActor)
{
	if (!CanAttackActor(TargetActor))
	{
		return;
	}
	
	if (CurrentAttackTarget == TargetActor)
	{
		return;
	}
	
	CurrentAttackTarget = TargetActor;
	BeginAttackWindUp();
}

void UDWCombatComponent::StopAutoAttack()
{
	if (CurrentAttackTarget && bShowDebugCombatMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		1.5f,
		FColor::Orange,
		FString::Printf(TEXT("Auto attack stopped: %s"), *CurrentAttackTarget->GetName())
		);
	}
	
	CurrentAttackTarget = nullptr;
	bIsAttackWindingUp = false;
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
	}
}

bool UDWCombatComponent::IsAutoAttacking() const
{
	return HasAttackTarget();
}

void UDWCombatComponent::BeginAttackWindUp()
{
	if (!CurrentAttackTarget)
	{
		StopAutoAttack();
		return;
	}
	
	if (!CanTargetActor(CurrentAttackTarget))
	{
		StopAutoAttack();
		return;
	}
	
	if (!CanAttackActor(CurrentAttackTarget))
	{
		StopAutoAttack();
		return;
	}
	
	bIsAttackWindingUp = true;
	
	OnAttackStarted.Broadcast(CurrentAttackTarget);
	
	if (bShowDebugCombatMessages && GEngine)
	{
		const FString OwnerName = GetOwner() ? GetOwner()->GetName() : TEXT("Unknown");
		const FString TargetName = CurrentAttackTarget ? CurrentAttackTarget->GetName() : TEXT("Unknown");

		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Yellow,
			FString::Printf(TEXT("%s winding up attack on %s"), *OwnerName, *TargetName)
		);
	}
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
		World->GetTimerManager().SetTimer(
		AutoAttackTimerHandle,
		this,
		&UDWCombatComponent::PerformAutoAttackTick,
		FMath::Max(0.0f, FirstAttackDelay),
		false
		);
	}
}

void UDWCombatComponent::PerformAutoAttackTick()
{
	if (!CurrentAttackTarget)
	{
		StopAutoAttack();
		return;
	}
	
	if (!CanTargetActor(CurrentAttackTarget))
	{
		StopAutoAttack();
		return;
	}
	
	bIsAttackWindingUp = false;
	
	if (!CanAttackActor(CurrentAttackTarget))
	{
		StopAutoAttack();
		return;
	}
	
	AttackActor(CurrentAttackTarget);
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
		World->GetTimerManager().SetTimer(
			AutoAttackTimerHandle,
			this,
			&UDWCombatComponent::BeginAttackWindUp,
			GetAttackCooldown(),
			false
		);
	}
}

bool UDWCombatComponent::IsTargetInAttackRange(AActor* TargetActor) const
{
	const AActor* Owner = GetOwner();
	if (!Owner || !TargetActor || TargetActor == Owner)
	{
		return false;
	}
	
	const float Distance = FVector::Dist2D(Owner->GetActorLocation(), TargetActor->GetActorLocation());
	return Distance <= GetAttackRange();
}

bool UDWCombatComponent::CanAttackActor(AActor* TargetActor) const
{
	return CanTargetActor(TargetActor) && IsTargetInAttackRange(TargetActor);
}

AActor* UDWCombatComponent::GetCurrentAttackTarget() const
{
	return CurrentAttackTarget;
}

bool UDWCombatComponent::HasAttackTarget() const
{
	return CurrentAttackTarget != nullptr;
}

void UDWCombatComponent::FaceTarget(AActor* TargetActor)
{
	AActor* Owner = GetOwner();
	if (!Owner || !TargetActor || TargetActor == Owner)
	{
		return;
	}
	
	FVector Direction = TargetActor->GetActorLocation() - Owner->GetActorLocation();
	Direction.Z = 0.0f;
	
	if (Direction.IsNearlyZero())
	{
		return;
	}
	
	const FRotator TargetRotation(0.0f, Direction.Rotation().Yaw, 0.0f);
	Owner->SetActorRotation(TargetRotation);
}

bool UDWCombatComponent::CanTargetActor(AActor* TargetActor) const
{
	AActor* Owner = GetOwner();
	if (!Owner || !TargetActor || TargetActor == Owner)
	{
		return false;
	}
	
	const UDWBodyHealthComponent* OwnerHealth = Owner->FindComponentByClass<UDWBodyHealthComponent>();
	if (OwnerHealth && OwnerHealth->IsIncapacitated())
	{
		return false;
	}
	
	const UDWBodyHealthComponent* TargetHealth = TargetActor->FindComponentByClass<UDWBodyHealthComponent>();
	if (!TargetHealth || TargetHealth->IsIncapacitated())
	{
		return false;
	}
	
	return true;
}

float UDWCombatComponent::GetAttackDamage() const
{
	return BaseAttackDamage * GetDamagerMultiplier();
}

float UDWCombatComponent::GetAttackCooldown() const
{
	return AttackCooldown / GetAttackSpeedMultiplier();
}

float UDWCombatComponent::GetAttackRange() const
{
	return AttackRange;
}

float UDWCombatComponent::GetAttackSpeedMultiplier() const
{
	return FMath::Max(0.1f, AttackSpeedMultiplier);
}

float UDWCombatComponent::GetDamagerMultiplier() const
{
	return FMath::Max(0.0f, DamageMultiplier);
}

bool UDWCombatComponent::IsAttackWingUp() const
{
	return bIsAttackWindingUp;
}

void UDWCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

