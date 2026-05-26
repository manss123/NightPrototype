#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DWDialogueTypes.generated.h"

USTRUCT(BlueprintType)
struct FDWDialogueLine
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName LineId = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Conditions")
	FGameplayTagContainer RequiredWorldTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Conditions")
	FGameplayTagContainer RequiredMemoryTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Conditions")
	FGameplayTagContainer RequiredContextTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 Priority = 0;
};
