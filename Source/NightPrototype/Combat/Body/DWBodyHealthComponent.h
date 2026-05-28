// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DWBodyHealthTypes.h"
#include "DWBodyHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDWOnBodyPartDamaged, FGameplayTag, BodyPartTag, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDWOnBodyPartDisabled, FGameplayTag, BodyPartTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDWOnBodyDamaged, FGameplayTag, BodyPartTag, float, DamageAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDWOnBodyDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDWOnBodyDowned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDWOnBodyRecovered);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWBodyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWBodyHealthComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	void ApplyDamageToBodyPart(FGameplayTag BodyPartTag, float DamageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	void HealBodyPart(FGameplayTag BodyPartTag, float HealAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	bool IsDead() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	bool IsBodyPartDisabled(FGameplayTag BodyPartTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	float GetBodyPartHealthPercent(FGameplayTag BodyPartTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	void ApplyDamageToCore(float DamageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	void ApplyDamageToRandomBodyPart(float DamageAmount);
	
	UFUNCTION(BlueprintPure, Category = "Body Health")
	bool IsAnyLegDisabled() const;
	
	UFUNCTION(BlueprintPure, Category = "Body Health")
	bool AreBothLegsDisabled() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	bool IsAnyArmDisabled() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	bool AreBothArmsDisabled() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	void SetDowned(bool bNewDowned);
	
	UFUNCTION(BlueprintCallable, Category = "Body Health")
	bool IsDowned() const;
	
	UFUNCTION(BlueprintPure, Category = "Body Health")
	EDWBodyState GetBodyState() const;
	
	UFUNCTION(BlueprintPure, Category = "Body Health")
	bool IsInjured() const;
	
	UFUNCTION(BlueprintPure, Category = "Body Health")
	bool IsIncapacitated() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health|Debug")
	void PrintBodyHealth() const;
	
	UFUNCTION(BlueprintPure, Category = "Body Health|Recovery")
	bool IsInRecoveryGracePeriod() const;
	
	UFUNCTION(BlueprintCallable, Category = "Body Health|Recovery")
	void StartRecoveryGracePeriod(float Duration);
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyPartDamaged OnBodyPartDamaged;
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyDamaged OnDamaged;
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyPartDisabled OnBodyPartDisabled;
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyDowned OnDowned;
	
	UPROPERTY(BlueprintAssignable, Category = "Body Health")
	FDWOnBodyRecovered OnRecovered;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body Health")
	TArray<FDWBodyPartHealth> BodyParts;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body Health", meta=(ToolTip = "If true, vital body parts reaching zero health will trigger Downed instead of Death. Useful for player characters or enemies that should be incapacitated before dying."))
	bool bDownedInsteadOfDeathOnVitalZero = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	bool bIsDead = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	bool bIsDowned = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	EDWBodyState BodyState = EDWBodyState::Healthy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	bool bCanRegenerate = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	float HealthRegenPerSecond = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Health")
	float DownedRecoveryHealthPercent = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body Health|Recovery")
	float DefaultRecoveryGracePeriod = 3.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Body Health|Recovery")
	bool bIsInRecoveryGracePeriod = false;

private:
	FDWBodyPartHealth* FindBodyPart(FGameplayTag BodyPartTag);
	const FDWBodyPartHealth* FindBodyPart(FGameplayTag BodyPartTag) const;
	
	void InitializeDefaultBodyParts();
	void RegenerateHealth(float DeltaTime);
	void TryRecoverFromDowned();
	
	FTimerHandle RecoveryGraceTimerHandle;
	
	void EndRecoveryGracePeriod();
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
};
