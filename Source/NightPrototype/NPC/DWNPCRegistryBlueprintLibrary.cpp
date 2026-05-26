// Fill out your copyright notice in the Description page of Project Settings.


#include "DWNPCRegistryBlueprintLibrary.h"
#include "DWNPCRegistrySubsystem.h"

UDWNPCRegistrySubsystem* UDWNPCRegistryBlueprintLibrary::GetNPCRegistry(const UObject* WorldContextObject)
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
	
	return World->GetSubsystem<UDWNPCRegistrySubsystem>();
}
