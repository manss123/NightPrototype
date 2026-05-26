// Fill out your copyright notice in the Description page of Project Settings.


#include "DWDayNightManager.h"
#include "Engine/Engine.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Encounter/DWEncounterDirector.h"
#include "Core/DWGameplayTags.h"
#include "GameplayTagContainer.h"

ADWDayNightManager::ADWDayNightManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADWDayNightManager::BeginPlay()
{
	Super::BeginPlay();
	
	bIsNight = CurrentHour >= 18 || CurrentHour < 6.0f;
}

static float GetSunAlpha(float Hour)
{
	if (Hour >= 4.0f && Hour < 8.0f)
	{
		return (Hour - 4.0f) / 4.0f;
	}

	if (Hour >= 8.0f && Hour < 16.0f)
	{
		return 1.0f;
	}

	if (Hour >= 16.0f && Hour < 20.0f)
	{
		return 1.0f - ((Hour - 16.0f) / 4.0f);
	}

	return 0.0f;
}

static float GetMoonAlpha(float Hour)
{
	if (Hour >= 16.0f && Hour < 20.0f)
	{
		return (Hour - 16.0f) / 4.0f;
	}

	if (Hour >= 20.0f || Hour < 4.0f)
	{
		return 1.0f;
	}

	if (Hour >= 4.0f && Hour < 8.0f)
	{
		return 1.0f - ((Hour - 4.0f) / 4.0f);
	}

	return 0.0f;
}

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
		
		const float SunAlpha = GetSunAlpha(CurrentHour);
		const float TargetIntensity = FMath::Lerp(NightSunIntensity, DaySunIntensity, SunAlpha);
		
		if (ULightComponent* LightComponent = SunLight->GetLightComponent())
		{
			LightComponent->SetIntensity(TargetIntensity);
		}
	}
	
	if (MoonLight)
	{
		const float MoonAlpha = GetMoonAlpha(CurrentHour);
		const float TargetIntensity = FMath::Lerp(DayMoonIntensity, NightMoonIntensity, MoonAlpha);

		MoonLight->SetActorRotation(FRotator(-45.0f, 135.0f, 0.0f));

		if (ULightComponent* LightComponent = MoonLight->GetLightComponent())
		{
			LightComponent->SetIntensity(TargetIntensity);
		}
	}
	
	if (SkyLight)
	{
		const float SunAlpha = GetSunAlpha(CurrentHour);
		const float BlendedSkyIntensity = FMath::Lerp(NightSkyIntensity, DaySkyIntensity, SunAlpha);
		const float TargetSkyIntensity = FMath::Max(MinimumSkyIntensity, BlendedSkyIntensity);

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
	
	RequestNightEncounters();

	OnNightStarted.Broadcast();
}

void ADWDayNightManager::RequestNightEncounters()
{
	if (!EncounterDirector)
	{
		if (bShowDebugMessage && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.0f,
				FColor::Orange,
				TEXT("No EncounterDirector assigned")
			);
		}
		
		return;
	}
	
	FGameplayTagContainer ContextTags;
	ContextTags.AddTag(DWGameplayTags::Context_Night);
		
	if (IsFirstNight())
	{
		ContextTags.AddTag(DWGameplayTags::Context_FirstNight);
		ContextTags.AddTag(DWGameplayTags::Context_IntroEncounter);
	}
		
	EncounterDirector->EvaluateEncountersWithGameplayContextAndTime(ContextTags, CurrentDay, CurrentHour);
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

FGameplayTagContainer ADWDayNightManager::GetCurrentContextTags() const
{
	FGameplayTagContainer ContextTags;
	
	if (bIsNight)
	{
		ContextTags.AddTag(DWGameplayTags::Context_Night);
	}
	
	if (IsFirstNight())
	{
		ContextTags.AddTag(DWGameplayTags::Context_FirstNight);
		ContextTags.AddTag(DWGameplayTags::Context_IntroEncounter);
	}
	
	return ContextTags;
}