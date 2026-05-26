// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DWEnemyAIComponent.generated.h"

UENUM(BlueprintType)
enum class EDWEnemmyAIState : uint8
{
	Idle,
	Chasing,
	Attacking
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWEnemyAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWEnemyAIComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
	void StartAI();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
	void StopAI();
	
	UFUNCTION(BlueprintPure, Category = "Enemy AI")
	bool IsAIActive() const;
	
	UFUNCTION(BlueprintPure, Category = "Enemy AI")
	EDWEnemmyAIState GetAIState() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	float DetectionRange = 1200.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	float LoseTargetRange = 1800.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	float ThinkInterval = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	bool bStartActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	float AttackRangeBuffer = 40.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy AI")
	bool bIsAIActive = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy AI")
	EDWEnemmyAIState AIState = EDWEnemmyAIState::Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI|Debug")
	bool bShowDebugMessage = true;
	
private:
	FTimerHandle ThinkTimerHandle;
	
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget = nullptr;
	
	void Think();
	void AcquirePlayerTarget();
	void ChaseTarget();
	void AttackTarget();
	
	bool IsTargetValid() const;
	bool ShouldKeepTarget() const;
	bool IsTargetInAttackRange() const;
};
