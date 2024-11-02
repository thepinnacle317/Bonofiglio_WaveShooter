// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyManager/EnemyWaveManager.h"

#include "EnemyAI/EnemyBase.h"
#include "EnemyAI/EnemyManager/EnemySpawner.h"

// Sets default values
AEnemyWaveManager::AEnemyWaveManager() :
/* Member Initializer List */
EnemyAliveThreshold(0),
InitialSpawnAmount(0),
AliveEnemyCount(0),
CurrentWaveIndex(0)
{
	PrimaryActorTick.bCanEverTick = false;
	AliveEnemyCount = 0;
}

void AEnemyWaveManager::StartNextWave()
{
	if (WaveDataTable)
	{
		// Get the row name based on the wave index.  *** Rows must be named "Wave1, Wave2, ... etc.
		FName RowName = FName(*FString::Printf(TEXT("Wave%d"), CurrentWaveIndex + 1));
		// Get the row name
		FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(RowName, TEXT(""));

		if (WaveData)
		{
			// Get the wave ready to start after the delay
			float Delay = WaveData->WaveDelay;
			GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, FTimerDelegate::CreateUObject(this,
				&AEnemyWaveManager::StartWave, CurrentWaveIndex), Delay, false);
		}
	}
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Start the first wave on begin with its delay
	StartNextWave();
}

void AEnemyWaveManager::CheckWaveCompletion()
{
	if (AliveEnemyCount <= 0)
	{
		// Handles Starting the next wave
		OnWaveEnd();
	}
}

void AEnemyWaveManager::StartWave(int32 WaveIndex)
{
	if (WaveDataTable)
	{
		// Construct the row name based on the wave index (e.g., Wave1, Wave2, ...)
		FName RowName = FName(*FString::Printf(TEXT("Wave%d"), WaveIndex + 1));
        
		// Retrieve the wave data from the data table
		FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(RowName, TEXT(""));

		if (WaveData)
		{
			// Store the current wave data
			CurrentWaveData = *WaveData;
			
			EnemyAliveThreshold = CurrentWaveData.EnemyAliveThreshold;

			// Reset remaining enemies for the current wave based on the defined enemy types
			RemainingEnemiesToSpawn = 0;
			for (const FEnemyTypeData& EnemyTypeData : CurrentWaveData.Enemies)
			{
				// Sum the enemy counts for the wave
				RemainingEnemiesToSpawn += EnemyTypeData.EnemyCount; 
			}

			// Reset the alive enemy count
			AliveEnemyCount = 0;

			// Spawn the initial enemies for this wave
			SpawnInitialEnemies(CurrentWaveData);
		}
	}

	// Increment the CurrentWaveIndex for the next wave
	CurrentWaveIndex++;
}

void AEnemyWaveManager::SpawnInitialEnemies(const FWaveData& WaveData)
{
	// Initialize the count of remaining enemies based on the initial spawn amount
	int32 RemainingEnemies = FMath::Min(InitialSpawnAmount, RemainingEnemiesToSpawn);

	// Iterate through the enemy types defined in the wave data
	for (const FEnemyTypeData& EnemyTypeData : WaveData.Enemies)
	{
		// Get the enemy class to spawn
		TSubclassOf<AEnemyBase> EnemyClass = EnemyTypeData.EnemyClass;

		// Calculate how many of this type to spawn based on the remaining enemies and defined count
		int32 SpawnCount = FMath::Min(RemainingEnemies, EnemyTypeData.EnemyCount);

		for (int32 i = 0; i < SpawnCount; i++)
		{
			// Check if there are spawners available to use
			if (EnemySpawners.Num() > 0)
			{
				// Cycle through spawners to distribute enemy spawns
				AEnemySpawner* Spawner = EnemySpawners[i % EnemySpawners.Num()];
				// Spawn the enemy using the spawner
				Spawner->SpawnEnemy(EnemyClass);
				

				// Increment the alive enemy count
				AliveEnemyCount++;

				RemainingEnemiesToSpawn--;

				// Decrement the remaining enemies
				RemainingEnemies--;

				// Bind the OnDeath event to track when the enemy dies
				AEnemyBase* LastEnemy = Spawner->GetLastSpawnedEnemy();
				if (LastEnemy)
				{
					LastEnemy->OnDeath.AddDynamic(this, &AEnemyWaveManager::OnEnemyDeath);
				}

				// If we run out of remaining enemies, break out of the loop
				if (RemainingEnemies <= 0) break;
			}
		}

		// If we have already spawned the initial amount of enemies, exit the loop early
		if (RemainingEnemies <= 0) break;
	}
}

void AEnemyWaveManager::CheckThenSpawnEnemies()
{
	// Check if there are remaining enemies to spawn and if the alive enemy count is below the threshold
	if (RemainingEnemiesToSpawn > 0 && AliveEnemyCount < EnemyAliveThreshold)
	{
		// Iterate through enemy types to spawn based on the current wave data
		for (const FEnemyTypeData& EnemyTypeData : CurrentWaveData.Enemies)
		{
			// Calculate how many enemies can be spawned based on the alive enemy threshold and the enemy count defined in the wave
			int32 SpawnCount = FMath::Min(EnemyAliveThreshold - AliveEnemyCount, EnemyTypeData.EnemyCount);

			// Adjust SpawnCount based on remaining enemies to spawn
			SpawnCount = FMath::Min(SpawnCount, RemainingEnemiesToSpawn);

			for (int32 i = 0; i < SpawnCount; i++)
			{
				// Get a spawner and spawn the enemy
				AEnemySpawner* Spawner = EnemySpawners[i % EnemySpawners.Num()];
				Spawner->SpawnEnemy(EnemyTypeData.EnemyClass);
                
				// Increment the count of alive enemies as they spawn
				AliveEnemyCount++;
				RemainingEnemiesToSpawn--;

				// Bind the OnDeath delegate for the newly spawned enemy
				AEnemyBase* LastEnemy = Spawner->GetLastSpawnedEnemy();
				if (LastEnemy)
				{
					LastEnemy->OnDeath.AddDynamic(this, &AEnemyWaveManager::OnEnemyDeath);
				}

				// Check if we have reached the alive enemy threshold
				if (AliveEnemyCount >= EnemyAliveThreshold) break;
			}

			// If the alive count threshold is met, break out of the enemy type loop
			if (AliveEnemyCount >= EnemyAliveThreshold) break;
		}
	}
}

void AEnemyWaveManager::OnEnemyDeath()
{
	// Remove from the alive enemy count
	AliveEnemyCount--;

	CheckWaveCompletion();
	
	// Spawn more enemies if available 
	CheckThenSpawnEnemies();
	
}

void AEnemyWaveManager::OnWaveEnd()
{
	// Will handle the delay between wave and begin the next round.
	StartNextWave();
}
