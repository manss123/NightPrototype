// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DWCombatAnimationComponent.generated.h"

class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWCombatAnimationComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UDWCombatAnimationComponent();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	TArray<TObjectPtr<UAnimMontage>> HitReactMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	float AttackMontagePlayRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	float HitReactMontagePlayRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	bool bUseAttackSpeedForAttackMontage = true;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Animation")
	int32 NextHitReactMontageIndex = 0;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Animation")
	int32 NextAttackMontageIndex = 0;
	
	UFUNCTION()
	void PlayNextAttackMontage(AActor* TargetActor);
	
	UFUNCTION()
	void PlayNextHitReactMontage(FGameplayTag BodyPartTag, float DamageAmount);
	
private:
	bool PlayMontageFromList(const TArray<TObjectPtr<UAnimMontage>>& Montages, int32& NextIndex, float PlayRate);
};
