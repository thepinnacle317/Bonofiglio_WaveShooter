// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerGameSave.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API UPlayerGameSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Progression")
	int32 Saved_WaveNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Progression")
	int32 Saved_TotalKills;
	
};
