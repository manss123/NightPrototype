// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/DWInteractableCharacter.h"
#include "Enemy/Data/DWEnemyProfileProvider.h"
#include "DWEnemyCharacter.generated.h"

class UDWEnemyProfileDataAsset;

UCLASS()
class NIGHTPROTOTYPE_API ADWEnemyCharacter : public ADWInteractableCharacter, public IDWEnemyProfileProvider
{
	GENERATED_BODY()

public:
	ADWEnemyCharacter();
	
	virtual UDWEnemyProfileDataAsset* GetEnemyProfile_Implementation() const override;
	virtual void InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action) override;
	
	virtual TArray<FDWInteractionOption> GetInteractionOptions_Implementation() override;
	virtual FText GetInteractText_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UDWEnemyProfileDataAsset> EnemyProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	float AttackDamageReceived = 25.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	bool bPrintBodyHealthAfterAttack = true;
	
private:
	void RegisterWithEnemyRoster();
	void UnregisterFromEnemyRoster();
	void HandleAttackInteraction(AActor* Interactor);
	bool IsEnemyDead() const;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
