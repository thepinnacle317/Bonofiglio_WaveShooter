// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveShooterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API UWaveShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWaveShooterGameInstance();
	
	UFUNCTION(BlueprintCallable, Category="Wave Game Properties")
	void IncrementWaveNumber();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wave Game Properties")
	int32 CurrentWaveNumber;
	
	UFUNCTION(BlueprintCallable, Category="Wave Game Properties")
	int32 GetWaveNumber() const { return CurrentWaveNumber; }
	
};
