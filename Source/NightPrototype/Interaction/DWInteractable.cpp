// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInteractable.h"
#include "GameFramework/Character.h"

bool IDWInteractable::CanInteract_Implementation(AActor* Interactor)
{
	return true;
}

void IDWInteractable::OnInteractFocusBegin_Implementation(AActor* Interactor)
{
	
}

void IDWInteractable::OnInteractFocusEnd_Implementation(AActor* Interactor)
{
	
}

FText IDWInteractable::GetDialogueText_Implementation()
{
	return FText::GetEmpty();
}

float IDWInteractable::GetInteractDistance_Implementation()
{
	return 0.0f;
}

bool IDWInteractable::ShouldFaceInteractor_Implementation()
{
	return false;
}

bool IDWInteractable::ShouldShowDialogue_Implementation()
{
	return false;
}

TArray<FDWInteractionOption> IDWInteractable::GetInteractionOptions_Implementation()
{
	TArray<FDWInteractionOption> Options;
	
	FDWInteractionOption PrimaryOption;
	PrimaryOption.Action = EDWInteractionAction::Primary;
	PrimaryOption.Label = FText::FromString(TEXT("Interact"));
	
	Options.Add(PrimaryOption);
	return Options;
}

void IDWInteractable::InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Option)
{
	Interact_Implementation(Interactor);
}

FVector IDWInteractable::GetActorFloorLocation(AActor* Actor)
{
	if (!Actor)
	{
		return FVector::ZeroVector;
	}
	
	FVector FloorLocation = Actor->GetActorLocation();
	
	if (const ACharacter* Character = Cast<ACharacter>(Actor))
	{
		FloorLocation.Z -= Character->GetSimpleCollisionHalfHeight();
	}
	
	return FloorLocation;
}
