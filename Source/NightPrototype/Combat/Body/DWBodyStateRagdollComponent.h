// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DWBodyStateRagdollComponent.generated.h"

class UDWBodyHealthComponent;
class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWBodyStateRagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWBodyStateRagdollComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll")
	bool bRagdollOnDeath = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll")
	bool bRagdollOnDowned = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll")
	FName RagdollCollisionProfileName = TEXT("Ragdoll");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	TObjectPtr<UAnimMontage> GetUpBackMontage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	TObjectPtr<UAnimMontage> GetUpFrontMontage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	float GetUpPlayRate = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	float GetUpFinishDelayBuffer = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	float RecoveryGraceBuffer = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragdoll|Recovery")
	FName RecoveryBoneName = TEXT("pelvis");
	
private:
	FTransform InitialMeshRelativeTransform;
	
	UFUNCTION()
	void HandleDeath();
	
	UFUNCTION()
	void HandleDowned();
	
	void EnterRagdoll();
	
	UFUNCTION()
	void HandleRecovered();
	
	void ExitRagdoll();
	void BeginGetUp();
	
	FTimerHandle GetUpTimerHandle;
	void FinishGetUp();
	
	UAnimMontage* SelectGetUpMontage() const;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
