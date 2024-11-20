// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyWaveData.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEnded);

/* Forward Declarations */
class UWaveShooterGameInstance;
class AEnemySpawner;

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

	UPROPERTY(BlueprintAssignable)
	FOnWaveEnded WaveEnded;

	/* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Data Core")
	TObjectPtr<UDataTable> WaveDataTable;

	/* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawners")
	TArray<AEnemySpawner*> EnemySpawners;

	/* Handles How many enemies can be alive at any given time.  Used to control pacing and difficulty *** This is set by WaveData */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Spawn Control")
	int32 EnemyAliveThreshold;

	/* The initial amount of enemies to spawn at the start of a wave *** This is set by WaveData */
	// TODO: Tie this to the WaveData Struct and set per wave 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Spawn Control")
	int32 InitialSpawnAmount;

	/* Used to determine how far to find spawners in range of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawn Control")
	float SpawnerRange;

	/* Used to determine how far to find spawners in range of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Manager | Spawn Control")
	float FallbackSpawnerRange;

	/* */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Manager | Spawn Control")
	int32 TotalEnemiesToSpawn;
	
	/* Used to start the next wave of enemies */
	void StartNextWave();

protected:
	virtual void BeginPlay() override;

	/* Used to check condition of the wave and determine the next action */
	void CheckWaveCompletion();

private:

	/* Used to hold and use the current waves data */
	FWaveData CurrentWaveData;

	/* Reference to the Wave Shooter Game Instance */
	TObjectPtr<UWaveShooterGameInstance> WaveGameInstance;

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

	/* */
	void StartWave(int32 WaveIndex);

	/* */
	void SpawnInitialEnemies(const FWaveData& WaveData);

	/* */
	void CheckThenSpawnEnemies();

	/* Used to find spawners that are within range of the player to use to spawn the enemy from */
	TArray<AEnemySpawner*> GetSpawnersInRange(float Range);

	/* */
	UFUNCTION()
	void OnEnemyDeath();

	/* */
	void OnWaveEnd();
};
