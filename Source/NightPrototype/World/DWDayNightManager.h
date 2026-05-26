// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DWDayNightManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDWDayNightEvent);

class ADirectionalLight;
class ASkyLight;

class ADWEncounterDirector;

UCLASS()
class NIGHTPROTOTYPE_API ADWDayNightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWDayNightManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Encounter")
	ADWEncounterDirector* EncounterDirector = nullptr;
	
	UFUNCTION(BlueprintPure, Category = "Day Night")
	FGameplayTagContainer GetCurrentContextTags() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void UpdateDayNightState();
	
	void HandleNightStarted();
	void HandleDayStarted();
	
	void RequestNightEncounters();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Debug")
	bool bShowDebugMessage = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Debug")
	bool bShowDebugTime = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Debug", meta = (ClampMin = "0.1"))
	float DebugTimePrintInterval = 2.0f;
	
	float TimeSinceLastDebugPrint = 0.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night")
	float CurrentHour = 8.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night")
	float DayLenghtSeconds = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night")
	bool bIsNight = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night")
	int32 CurrentDay = 1;
	
	UPROPERTY(BlueprintAssignable, Category = "Day Night|Events")
	FDWDayNightEvent OnNightStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Day Night|Events")
	FDWDayNightEvent OnDayStarted;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	ADirectionalLight* SunLight = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float DaySunIntensity = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float NightSunIntensity = 0.05f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	ADirectionalLight* MoonLight = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float DayMoonIntensity = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float NightMoonIntensity = 0.35f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	ASkyLight* SkyLight = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float DaySkyIntensity = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float NightSkyIntensity = 0.15f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night|Lighting")
	float MinimumSkyIntensity = 0.35f;
	
	UFUNCTION(BlueprintPure, Category = "Day Night")
	FText GetTimeText() const;
	
	UFUNCTION(BlueprintPure, Category = "Day Night")
	FText GetDayTimeText() const;
	
	UFUNCTION(BlueprintPure, Category = "Day Night")
	bool IsFirstNight() const;
};
