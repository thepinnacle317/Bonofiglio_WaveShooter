// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyManager/EnemyWaveManager.h"
#include "EnemyAI/EnemyBase.h"
#include "EnemyAI/EnemyManager/EnemySpawner.h"
#include "GameCore/WaveShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyWaveManager::AEnemyWaveManager() :
/* Member Initializer List */
EnemyAliveThreshold(0),
InitialSpawnAmount(0),
AliveEnemyCount(0),
CurrentWaveIndex(0),
RemainingEnemiesToSpawn(0),
SpawnerRange(2000.f),
FallbackSpawnerRange(5000.f)
{
	PrimaryActorTick.bCanEverTick = false;
	AliveEnemyCount = 0;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	WaveGameInstance = Cast<UWaveShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!WaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveGameInstance is null. Make sure the game instance is set in the project settings."));
		return;
	}
	
	// Start the first wave on begin with its delay
	StartNextWave();
}

void AEnemyWaveManager::StartNextWave()
{
	if (WaveDataTable && WaveGameInstance)
	{
		// Get the wave number from the Game Instance
		CurrentWaveIndex = WaveGameInstance->GetWaveNumber();
		
		// Get the row name based on the wave index.  *** Rows must be named "Wave1, Wave2, ... etc.
		FName RowName = FName(*FString::Printf(TEXT("Wave%d"), CurrentWaveIndex));
		// Get the row name
		FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(RowName, TEXT(""));

		if (WaveData)
		{
			// Get the wave ready to start after the delay
			float Delay = WaveData->WaveDelay;
			GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, FTimerDelegate::CreateUObject(this,
				&AEnemyWaveManager::StartWave, CurrentWaveIndex), Delay, false);

			// Increment the CurrentWaveIndex for the next wave
			WaveGameInstance->IncrementWaveNumber();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Wave data not found for Wave%d"), CurrentWaveIndex);
		}
	}
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
		FName RowName = FName(*FString::Printf(TEXT("Wave%d"), WaveIndex));
        
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
}

void AEnemyWaveManager::SpawnInitialEnemies(const FWaveData& WaveData)
{
	// Check if there are remaining enemies to spawn and if the alive enemy count is below the threshold
    if (RemainingEnemiesToSpawn > 0 && AliveEnemyCount < EnemyAliveThreshold)
    {
        // Get the spawners that are within range of the player
        TArray<AEnemySpawner*> SpawnersNearby = GetSpawnersInRange(SpawnerRange);
        if (SpawnersNearby.Num() == 0)
        {
            // If no spawners nearby, use fallback spawners
            SpawnersNearby = EnemySpawners;
        }

        // Iterate through enemy types to spawn based on the current wave data
        for (const FEnemyTypeData& EnemyTypeData : CurrentWaveData.Enemies)
        {
            // Calculate how many enemies of this type can be spawned based on the alive enemy threshold and remaining count
            int32 SpawnableCount = FMath::Min(EnemyAliveThreshold - AliveEnemyCount, EnemyTypeData.EnemyCount);

            // Adjust SpawnCount based on remaining enemies to spawn
            int32 SpawnCount = FMath::Min(SpawnableCount, RemainingEnemiesToSpawn);

            for (int32 i = 0; i < SpawnCount; i++)
            {
                // Get a spawner and spawn the enemy
                AEnemySpawner* Spawner = SpawnersNearby[i % SpawnersNearby.Num()];
                Spawner->SpawnEnemy(EnemyTypeData.EnemyClass);

                // Increment the count of alive enemies as they spawn
                AliveEnemyCount++;
                RemainingEnemiesToSpawn--;
                const_cast<FEnemyTypeData&>(EnemyTypeData).EnemyCount--; // Reduce this enemy type's count

                // Bind the OnDeath delegate for the newly spawned enemy
                AEnemyBase* LastEnemy = Spawner->GetLastSpawnedEnemy();
                if (LastEnemy)
                {
                    LastEnemy->OnDeath.AddDynamic(this, &AEnemyWaveManager::OnEnemyDeath);
                }

                // If we reach the alive enemy threshold, stop spawning
                if (AliveEnemyCount >= EnemyAliveThreshold)
                {
                    return; // Exit the function early, but keep remaining enemy counts intact
                }
            }
        }
    }
}

void AEnemyWaveManager::CheckThenSpawnEnemies()
{
	// Check if there are remaining enemies to spawn and if the alive enemy count is below the threshold
	if (RemainingEnemiesToSpawn > 0 && AliveEnemyCount < EnemyAliveThreshold)
	{
		// Get the spawners that are within range of the player
		TArray<AEnemySpawner*> SpawnersNearby = GetSpawnersInRange(SpawnerRange);
		
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
				AEnemySpawner* Spawner = SpawnersNearby[i % SpawnersNearby.Num()];
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

TArray<AEnemySpawner*> AEnemyWaveManager::GetSpawnersInRange(float Range)
{
	TArray<AEnemySpawner*> SpawnersNearPlayer;

	// Get the location of the player to use as the point to search from
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter) return SpawnersNearPlayer;

	// Get the players current location
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	// Iterate through all the spawners and check if they are within the range threshold of the player
	// to be used to spawn an enemy at
	for (AEnemySpawner* Spawner : EnemySpawners)
	{
		if (Spawner && FVector::Dist(PlayerLocation, Spawner->GetActorLocation()) <= Range)
		{
			// Add the spawner that passed the check to the local spawner array
			SpawnersNearPlayer.Add(Spawner);
		}
	}

	// If there are no spawners near the player then return the full list of spawners
	if (SpawnersNearPlayer.Num() == 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("No spawners within defined range! Returning full list"));
		}
		return EnemySpawners;
	}

	return SpawnersNearPlayer;
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
	if (WaveEnded.IsBound())
	{
		WaveEnded.Broadcast();
	}
}
