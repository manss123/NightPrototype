// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInteractableActor.h"
#include "Engine/Engine.h"

// Sets default values
ADWInteractableActor::ADWInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	InteractionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPoint"));
	InteractionPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADWInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADWInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FDWInteractionOption> ADWInteractableActor::GetInteractionOptions_Implementation()
{
	if (InteractionOptions.Num() > 0)
	{
		return InteractionOptions;
	}
	
	return IDWInteractable::GetInteractionOptions_Implementation();
}

FVector ADWInteractableActor::GetInteractLocation_Implementation()
{
	if (InteractionPoint)
	{
		return InteractionPoint->GetComponentLocation();
	}
	
	return GetActorLocation();
}

FText ADWInteractableActor::GetInteractText_Implementation()
{
	return InteractText;
}

float ADWInteractableActor::GetInteractDistance_Implementation()
{
	return InteractDistance;
}

bool ADWInteractableActor::CanInteract_Implementation(AActor* Interactor)
{
	return bCanInteract;
}

bool ADWInteractableActor::ShouldShowDialogue_Implementation()
{
	return bShowDialogueOnInteract;
}

FText ADWInteractableActor::GetDialogueText_Implementation()
{
	return DialogueText;
}

void ADWInteractableActor::InteractWithOption_Implementation(AActor* Interactor, EDWInteractionAction Action)
{
	if (!ShowDebugInteractionMessage || !GEngine)
	{
		return;
	}
	
	const FString ActionName = UEnum::GetValueAsString(Action);
	const FString ActorName = GetName();
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Green,
		FString::Printf(TEXT("%s: %s"), *ActorName, *ActionName)
		);
}

FVector ADWInteractableActor::GetInteractLocationForInteractor(AActor* Interactor)
{
	if (!bUseDynamicInteractLocation || !Interactor)
	{
		return GetInteractLocation_Implementation();
	}
	
	const FVector ActorFloorLocation = IDWInteractable::GetActorFloorLocation(this);
	const FVector InteractFloorLocation = IDWInteractable::GetActorFloorLocation(Interactor);
	
	FVector Direction = InteractFloorLocation - ActorFloorLocation;
	Direction.Z = 0.0f;
	
	if (Direction.IsNearlyZero())
	{
		return GetInteractLocation_Implementation();
	}
	
	return ActorFloorLocation + Direction.GetSafeNormal() * DynamicInteractDistance;
}
