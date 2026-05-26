// Fill out your copyright notice in the Description page of Project Settings.


#include "DWNemesisBlueprintLibrary.h"
#include "DWNemesisSubsystem.h"

UDWNemesisSubsystem* UDWNemesisBlueprintLibrary::GetNemesisSubsystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UDWNemesisSubsystem>();
		}
	}
	
	return nullptr;
}
