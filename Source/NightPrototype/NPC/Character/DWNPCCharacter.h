// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/DWInteractableCharacter.h"
#include "NPC/DWNPCProfileProvider.h"
#include "GameplayTagContainer.h"
#include "DWNPCCharacter.generated.h"

class UDWNPCProfileDataAsset;
class UDWMemoryComponent;
class ADWDayNightManager;
class UDWBodyHealthComponent;
class UDWBodyStateRagdollComponent;

UCLASS()
class NIGHTPROTOTYPE_API ADWNPCCharacter : public ADWInteractableCharacter, public IDWNPCProfileProvider
{
	
	GENERATED_BODY()
public:
	ADWNPCCharacter();
	
	virtual UDWNPCProfileDataAsset* GetNPCProfile_Implementation() const override;
	
	virtual TArray<FDWInteractionOption> GetInteractionOptions_Implementation() override;
	virtual FText GetInteractText_Implementation() override;
	virtual bool ShouldShowDialogue_Implementation() override;
	virtual FText GetDialogueText_Implementation() override;
	virtual void InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UDWNPCProfileDataAsset> NPCProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Dialogue")
	FText FallbackDialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Dialogue")
	TObjectPtr<ADWDayNightManager> DayNightManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Debug")
	bool bShowDebugNPCInteraction = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Memory")
	TObjectPtr<UDWMemoryComponent> MemoryComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Memory")
	TObjectPtr<UDWBodyHealthComponent> BodyHealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Memory")
	TObjectPtr<UDWBodyStateRagdollComponent> BodyStateRagdollComponent;
	
private:
	void RegisterWithNPCRegistry();
	void UnregisterFromNPCRegistry();
	
	FGameplayTagContainer GetWorldDialogueTags() const;
	FGameplayTagContainer GetDialogueContextTags() const;
};
