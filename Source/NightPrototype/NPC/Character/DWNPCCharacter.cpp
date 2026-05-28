// Fill out your copyright notice in the Description page of Project Settings.


#include "DWNPCCharacter.h"

#include "NPC/DWNPCProfileDataAsset.h"
#include "NPC/DWNPCRegistrySubsystem.h"
#include "Engine/Engine.h"
#include "Memory/DWMemoryComponent.h"
#include "Core/DWGameInstance.h"
#include "Dialogue/DWDialogueSelectorLibrary.h"
#include "Dialogue/DWDialogueTypes.h"
#include "World/DWDayNightManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/DWGameplayTags.h"
#include "Combat/Body/DWBodyHealthComponent.h"
#include "Combat/Body/DWBodyStateRagdollComponent.h"

ADWNPCCharacter::ADWNPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MemoryComponent = CreateDefaultSubobject<UDWMemoryComponent>("MemoryComponent");
	BodyHealthComponent = CreateDefaultSubobject<UDWBodyHealthComponent>("BodyHealthComponent");
	BodyStateRagdollComponent = CreateDefaultSubobject<UDWBodyStateRagdollComponent>("BodyStateRagdollComponent");
	
	InteractText = FText::FromString(TEXT("Talk"));
	InteractDistance = 150.0f;
	bCanInteract = true;
	bShouldFaceInteractor = true;
	bUseDynamicInteractLocation = true;
	DynamicInteractDistance = 150.0f;
	bShowDialogueOnInteract = true;
	
	FallbackDialogueText = FText::FromString(TEXT("..."));
}

void ADWNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!DayNightManager)
	{
		DayNightManager = Cast<ADWDayNightManager>(UGameplayStatics::GetActorOfClass(this, ADWDayNightManager::StaticClass()));
	}
	
	RegisterWithNPCRegistry();
}

void ADWNPCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromNPCRegistry();
	
	Super::EndPlay(EndPlayReason);
}

UDWNPCProfileDataAsset* ADWNPCCharacter::GetNPCProfile_Implementation() const
{
	return NPCProfile;
}

TArray<FDWInteractionOption> ADWNPCCharacter::GetInteractionOptions_Implementation()
{
	TArray<FDWInteractionOption> Options;
	
	FDWInteractionOption TalkOption;
	TalkOption.Action = EDWInteractionAction::Talk;
	TalkOption.Label = FText::FromString(TEXT("Talk"));
	
	Options.Add(TalkOption);
	return Options;
}

FText ADWNPCCharacter::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Talk"));
}

bool ADWNPCCharacter::ShouldShowDialogue_Implementation()
{
	return  true;
}

FText ADWNPCCharacter::GetDialogueText_Implementation()
{
	if (NPCProfile && NPCProfile->DialogueBank)
	{
		const FGameplayTagContainer WorldTags = GetWorldDialogueTags();
		const FGameplayTagContainer ContextTags = GetDialogueContextTags();
		
		UDWGameInstance* DWGameInstance = nullptr;

		if (UWorld* World = GetWorld())
		{
			DWGameInstance = World->GetGameInstance<UDWGameInstance>();
		}
		
		FDWDialogueLine SelectedLine;

		if (UDWDialogueSelectorLibrary::SelectBestDialogueLine(
			NPCProfile->DialogueBank,
			WorldTags,
			MemoryComponent,
			ContextTags,
			DWGameInstance,
			SelectedLine
		))
		{
			return SelectedLine.Text;
		}
	}
	
	return FallbackDialogueText;
}

void ADWNPCCharacter::InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action)
{
	if (Action != EDWInteractionAction::Talk && Action != EDWInteractionAction::Primary)
	{
		Super::InteractWithOption_Implementation(Interactor, Action);
		return;
	}
	
	if (MemoryComponent)
	{
		MemoryComponent->AddMemoryByTag(DWGameplayTags::Memory_NPC_PlayerTalkedToMe);
	}
	
	if (bShowDebugNPCInteraction && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Green,
		FString::Printf(TEXT("%s talked to %s"), *GetNameSafe(Interactor), *GetName())
		);
	}
}

FGameplayTagContainer ADWNPCCharacter::GetWorldDialogueTags() const
{
	FGameplayTagContainer WorldTags;
	
	if (const UWorld* World = GetWorld())
	{
		if (const UDWGameInstance* DWGameInstance = World->GetGameInstance<UDWGameInstance>())
		{
			WorldTags = DWGameInstance->GetWorldGameplayTags();
		}
	}
	
	return WorldTags;
}

FGameplayTagContainer ADWNPCCharacter::GetDialogueContextTags() const
{
	if (DayNightManager)
	{
		return DayNightManager->GetCurrentContextTags();
	}
	
	return FGameplayTagContainer();
}

void ADWNPCCharacter::RegisterWithNPCRegistry()
{
	if (UWorld* World = GetWorld())
	{
		if (UDWNPCRegistrySubsystem* NPCRegistry = World->GetSubsystem<UDWNPCRegistrySubsystem>())
		{
			NPCRegistry->RegisterNPC(this);
		}
	}
}

void ADWNPCCharacter::UnregisterFromNPCRegistry()
{
	if (UWorld* World = GetWorld())
	{
		if (UDWNPCRegistrySubsystem* NPCRegistry = World->GetSubsystem<UDWNPCRegistrySubsystem>())
		{
			NPCRegistry->UnregisterNPC(this);
		}
	}
}
