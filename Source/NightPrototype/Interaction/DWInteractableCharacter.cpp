// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInteractableCharacter.h"
#include "Engine/Engine.h"

ADWInteractableCharacter::ADWInteractableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InteractionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPoint"));
	InteractionPoint->SetupAttachment(RootComponent);
}

void ADWInteractableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADWInteractableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FDWInteractionOption> ADWInteractableCharacter::GetInteractionOptions_Implementation()
{
	if (InteractionOptions.Num() > 0)
	{
		return InteractionOptions;
	}
	
	return IDWInteractable::GetInteractionOptions_Implementation();
}

FVector ADWInteractableCharacter::GetInteractLocation_Implementation()
{
	if (InteractionPoint)
	{
		return InteractionPoint->GetComponentLocation();
	}
	
	return IDWInteractable::GetActorFloorLocation(this);
}

FText ADWInteractableCharacter::GetInteractText_Implementation()
{
	return InteractText;
}

float ADWInteractableCharacter::GetInteractDistance_Implementation()
{
	return InteractDistance;
}

bool ADWInteractableCharacter::CanInteract_Implementation(AActor* Interactor)
{
	return bCanInteract;
}

bool ADWInteractableCharacter::ShouldFaceInteractor_Implementation()
{
	return bShouldFaceInteractor;
}

bool ADWInteractableCharacter::ShouldShowDialogue_Implementation()
{
	return bShowDialogueOnInteract;
}

FText ADWInteractableCharacter::GetDialogueText_Implementation()
{
	return DialogueText;
}

void ADWInteractableCharacter::InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action)
{
	if (!bShowDebugInteractionMessage || !GEngine)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Green,
		FString::Printf(TEXT("%s: %s"), *GetName(), *UEnum::GetValueAsString(Action))
	);
}

FVector ADWInteractableCharacter::GetInteractLocationForInteractor(AActor* Interactor)
{
	if (!bUseDynamicInteractLocation || !Interactor)
	{
		return GetInteractLocation_Implementation();
	}
	
	const FVector ActorFloorLocation = IDWInteractable::GetActorFloorLocation(this);
	const FVector InteractorFloorLocation = IDWInteractable::GetActorFloorLocation(Interactor);
	
	FVector Direction = InteractorFloorLocation - ActorFloorLocation;
	Direction.Z = 0.0f;
	
	if (Direction.IsNearlyZero())
	{
		return GetInteractLocation_Implementation();
	}
	
	return ActorFloorLocation + Direction.GetSafeNormal() * DynamicInteractDistance;
}

void ADWInteractableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

