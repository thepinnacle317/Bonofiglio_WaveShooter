// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStates.h"
#include "GameFramework/GameStateBase.h"
#include "WaveShooterGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AWaveShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Properties")
	EGameStates CurrentGameState;
	
};
