// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyRosterBlueprintLibrary.h"
#include "Enemy/Roster/DWEnemyRosterSubsystem.h"

UDWEnemyRosterSubsystem* UDWEnemyRosterBlueprintLibrary::GetEnemyRoster(const UObject* WorldContextObject)
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
	
	return World->GetSubsystem<UDWEnemyRosterSubsystem>();
}
