// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DWMemoryTypes.h"
#include "DWMemoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NIGHTPROTOTYPE_API UDWMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDWMemoryComponent();
	
	UFUNCTION(Blueprintable, Category = "Memory")
	void AddMemory(const FDWMemoryEntry& MemoryEntry);
	
	UFUNCTION(BlueprintPure, Category = "Memory")
	bool HasMemory(FGameplayTag MemoryTag) const;
	
	UFUNCTION(BlueprintPure, Category = "Memory")
	const TArray<FDWMemoryEntry>& GetMemories() const;
	
	UFUNCTION(BlueprintCallable, Category = "Memory")
	void ClearMemories();
	
	UFUNCTION(BlueprintCallable, Category = "Memory")
	void AddMemoryByTag(FGameplayTag MemoryTag);
	
	UFUNCTION(BlueprintCallable, Category = "Memory|Debug")
	void PrintMemories() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Memory")
	TArray<FDWMemoryEntry> Memories;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
