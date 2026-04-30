// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInteractable.h"

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
