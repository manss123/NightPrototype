
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "DWCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDWOnAttackStartedSignature, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDWOnAttackHitSignature, AActor*, TargetActor, float, DamageAmount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWCombatComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackActor(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanAttackActor(AActor* TargetActor) const;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAutoAttack(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAutoAttack();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAutoAttacking() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsAttackWingUp() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	AActor* GetCurrentAttackTarget() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool HasAttackTarget() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetAttackDamage() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetAttackCooldown() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float FirstAttackDelay = 0.25f;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetAttackRange() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetAttackSpeedMultiplier() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetDamagerMultiplier() const;
	
	bool CanTargetActor(AActor* TargetActor) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FDWOnAttackStartedSignature OnAttackStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FDWOnAttackHitSignature OnAttackHit;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseAttackDamage = 25.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bPrintTargetHealthAfterAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown = 0.8f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackSpeedMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackRange = 180.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CurrentAttackTarget = nullptr;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	bool bIsAttackWindingUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bFaceTargetOnAttack = true;
	
	FTimerHandle AutoAttackTimerHandle;

	void BeginAttackWindUp();
	void PerformAutoAttackTick();

	bool IsTargetInAttackRange(AActor* TargetActor) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Debug")
	bool bShowDebugCombatMessages = false;
	
private:
	void FaceTarget(AActor* TargetActor);
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
