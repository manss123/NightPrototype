#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWNemesisTypes.generated.h"

USTRUCT(BlueprintType)
struct FDWNemesisState
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	FName EnemyId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	FGameplayTag NemesisTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	int32 EncounterCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	float ScarSeverity = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	bool bHasDefeatedPlayer = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nemesis")
	bool bWasDefeatedByPlayer = false;
};
