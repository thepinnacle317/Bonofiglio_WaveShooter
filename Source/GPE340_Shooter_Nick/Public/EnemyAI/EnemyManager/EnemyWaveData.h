#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyWaveData.generated.h"

class AEnemyBase;

USTRUCT(BlueprintType)
struct FEnemyTypeData : public FTableRowBase
{
	GENERATED_BODY()


	/* The amount of enemies to spawn for the specified wave of this class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Data | Spawn Count")
	int32 EnemyCount;

	/* Enemy class of enemy to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Data | Class Type")
	TSubclassOf<AEnemyBase> EnemyClass;

	// Initialize the data with a constructor
	FEnemyTypeData() :
	EnemyCount(0),
	EnemyClass(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

public:

	/* Array of different enemy types and the amount to spawn for the wave specified by row */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemies to Spawn by Type")
	TArray<FEnemyTypeData> Enemies;

	/* The Delay before starting the next wave *** in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Wave Parameters")
	float WaveDelay;

	/* Threshold for the alive enemies before spawning */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Wave Parameters")
	int32 InitialSpawnAmount;

	/* The delay between when an enemy is spawned at a found spawner */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Wave Parameters")
	float SpawnerDelay;

	/* Threshold for the alive enemies before spawning */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Wave Parameters")
	int32 EnemyAliveThreshold;

	FWaveData():
	WaveDelay(0.0f),
	InitialSpawnAmount(0),
	SpawnerDelay(0.0f),
	EnemyAliveThreshold(0)
	
	{
	}
};
