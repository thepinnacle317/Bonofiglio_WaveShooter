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

	/* Used to track and display the total number of enemies the player has killed after they die */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wave Game Properties")
	int32 TotalEnemiesKilled;

	/// Game User Settings ///

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	float PlayerFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	float MouseSensitivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	float MainVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	float MusicVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	float SFXVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	bool InvertMouse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	bool ResolutionSettingsMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	bool UseModifiedMouse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	FString Resolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	FString ResolutionQualityPercentage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="User Game Settings")
	bool UseFullScreen;
};
