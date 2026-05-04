// Fill out your copyright notice in the Description page of Project Settings.


#include "DWDayNightManager.h"
#include "Engine/Engine.h"
#include "DWGameInstance.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"

// Sets default values
ADWDayNightManager::ADWDayNightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADWDayNightManager::BeginPlay()
{
	Super::BeginPlay();
	
	bIsNight = CurrentHour >= 18 || CurrentHour < 6.0f;
}

// Called every frame
void ADWDayNightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (DayLenghtSeconds <= 0.0f)
	{
		return;
	}
	
	const float HoursPerSecond = 24.0f / DayLenghtSeconds;
	CurrentHour += DeltaTime * HoursPerSecond;
	
	while (CurrentHour >= 24.0f)
	{
		CurrentHour -= 24.0f;
		CurrentDay++;
	}
	
	UpdateDayNightState();
	
	if (SunLight)
	{
		const float SunPitch = -((CurrentHour - 6.0f) / 24.0f) * 360.0f;
		SunLight->SetActorRotation(FRotator(SunPitch, -45.0f, 0.0f));
		
		const float TargetIntensity = bIsNight ? NightSunIntensity : DaySunIntensity;
		
		if (ULightComponent* LightComponent = SunLight->GetLightComponent())
		{
			LightComponent->SetIntensity(TargetIntensity);
		}
	}
	
	if (SkyLight)
	{
		const float TargetSkyIntensity = bIsNight ? NightSkyIntensity : DaySkyIntensity;

		if (USkyLightComponent* SkyLightComponent = SkyLight->GetLightComponent())
		{
			SkyLightComponent->SetIntensity(TargetSkyIntensity);
		}
	}
	
	if (bShowDebugTime)
	{
		TimeSinceLastDebugPrint += DeltaTime;

		if (TimeSinceLastDebugPrint >= DebugTimePrintInterval)
		{
			TimeSinceLastDebugPrint = 0.0f;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					DebugTimePrintInterval,
					FColor::Cyan,
					FString::Printf(TEXT("Time: %s"), *GetDayTimeText().ToString())
				);
			}
		}
	}
}

void ADWDayNightManager::UpdateDayNightState()
{
	const bool bNewIsNight = CurrentHour >= 18 || CurrentHour < 6.0f;
	
	if (bNewIsNight == bIsNight)
	{
		return;
	}
	
	bIsNight = bNewIsNight;
	
	if (bIsNight)
	{
		HandleNightStarted();
	}
	else
	{
		HandleDayStarted();
	}
}

void ADWDayNightManager::HandleNightStarted()
{
	if (bShowDebugMessage && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Purple, TEXT("Night has started"));
	}

	if (IsFirstNight() && !bHasTriggeredFirstNight)
	{
		bHasTriggeredFirstNight = true;
		if (UDWGameInstance* DWGameInstance = GetGameInstance<UDWGameInstance>())
		{
			DWGameInstance->AddWorldEvent(UDWGameInstance::Event_FirstNightStarted);
			DWGameInstance->PrintWorldEvents();
		}
		
		for (AActor* ActorToReveal : FirstNightActorsToReveal)
		{
			if (!ActorToReveal)
			{
				continue;
			}
			
			ActorToReveal->SetActorHiddenInGame(false);
			ActorToReveal->SetActorEnableCollision(true);
			ActorToReveal->SetActorTickEnabled(true);
		}
		
		if (bShowDebugMessage && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.0f, FColor::Red, TEXT("Something has appeared outside the village"));
		}
	}

	OnNightStarted.Broadcast();
}
void ADWDayNightManager::HandleDayStarted()
{
	if (bShowDebugMessage && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Purple, TEXT("Day has started"));
	}
	
	OnDayStarted.Broadcast();
}

FText ADWDayNightManager::GetTimeText() const
{
	const int32 Hour = FMath::FloorToInt(CurrentHour);
	const int32 Minute = FMath::FloorToInt((CurrentHour - Hour) * 60.0);
	
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Hour, Minute));
}

FText ADWDayNightManager::GetDayTimeText() const
{
	return FText::FromString(
		FString::Printf(TEXT("Day %d - %s"), CurrentDay, *GetTimeText().ToString())
		);
}

bool ADWDayNightManager::IsFirstNight() const
{
	return CurrentDay == 1 && bIsNight;
}
