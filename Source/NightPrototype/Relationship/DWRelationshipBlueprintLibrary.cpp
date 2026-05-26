// Fill out your copyright notice in the Description page of Project Settings.


#include "DWRelationshipBlueprintLibrary.h"
#include "DWRelationshipSubsystem.h"
#include "Kismet/GameplayStatics.h"

UDWRelationshipSubsystem* UDWRelationshipBlueprintLibrary::GetRelationshipSubsystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}
	
	return World->GetSubsystem<UDWRelationshipSubsystem>();
}
