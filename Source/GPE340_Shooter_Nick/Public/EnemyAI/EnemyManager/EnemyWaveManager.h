// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyWaveData.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveManager.generated.h"

class AEnemySpawner;

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

	/* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Data Core")
	TObjectPtr<UDataTable> WaveDataTable;

	/* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawners")
	TArray<AEnemySpawner*> EnemySpawners;

	/* Handles How many enemies can be alive at any given time.  Used to control pacing and difficulty*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawn Control")
	int32 EnemyAliveThreshold;

	/* The initial amount of enemies to spawn at the start of a wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawn Control")
	int32 InitialSpawnAmount;

	

	/* Used to start the next wave of enemies */
	void StartNextWave();

protected:
	virtual void BeginPlay() override;

	/* Used to check condition of the wave and determine the next action */
	void CheckWaveCompletion();

private:
	FWaveData CurrentWaveData;

	/* Timer handle used to manage the wave timer */
	FTimerHandle WaveTimerHandle;

	/* Used to track the current amount of enemies alive */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Wave Data", meta = (AllowPrivateAccess = "true"))
	int32 AliveEnemyCount;

	/* Index used to track the current wave the player is on */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Wave Data", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveIndex;

	/* Track the amount of enemies that are left to spawn for the wave */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Wave Data", meta = (AllowPrivateAccess = "true"))
	int32 RemainingEnemiesToSpawn;

	void StartWave(int32 WaveIndex);
	
	void SpawnInitialEnemies(const FWaveData& WaveData);
	
	void CheckThenSpawnEnemies();
	
	UFUNCTION()
	void OnEnemyDeath();
	
	void OnWaveEnd();
};
