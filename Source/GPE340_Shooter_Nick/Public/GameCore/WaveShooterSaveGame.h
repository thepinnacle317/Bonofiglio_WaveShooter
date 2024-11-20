// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WaveShooterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API UWaveShooterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	float Saved_PlayerFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	float Saved_MouseSensitivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	float Saved_MainVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	float Saved_MusicVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	float Saved_SFXVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	bool Saved_InvertMouse;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	bool Saved_ResolutionSettingsMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	bool Saved_UseModifiedMouse;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	FString Saved_Resolution;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	FString Saved_ResolutionQualityPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	bool Saved_VSync;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	bool Saved_FullScreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_Shadows;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_Foliage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_Textures;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_FXAA;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_Effects;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="User Game Settings")
	int32 Saved_ResolutionQuality;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Progression")
	int32 Saved_WaveNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Progression")
	int32 Saved_TotalKills;
	
};
