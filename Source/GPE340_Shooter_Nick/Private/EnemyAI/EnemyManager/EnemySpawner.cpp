// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyManager/EnemySpawner.h"

#include "AIController.h"
#include "EnemyAI/EnemyBase.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	LastSpawnedEnemy = nullptr;
}

void AEnemySpawner::SpawnEnemy_Implementation(TSubclassOf<AEnemyBase> EnemyClass)
{
	if (!EnemyClass) return;

	for (int32 Attempt = 0; Attempt < MaxSpawnAttempts; Attempt++)
	{
		FVector SpawnLocation = GetActorLocation();
		if (Attempt > 0)
		{
			SpawnLocation += FVector(FMath::RandRange(-SpawnRadius, SpawnRadius),
									  FMath::RandRange(-SpawnRadius, SpawnRadius), 0.0f);
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, SpawnLocation, GetActorRotation(), SpawnParams);
		if (SpawnedEnemy)
		{
			LastSpawnedEnemy = SpawnedEnemy;

			// Retrieve the AI controller class from the enemy's defaults
			TSubclassOf<AAIController> EnemyAIControllerClass = LastSpawnedEnemy->EnemyAIControllerClass;

			if (EnemyAIControllerClass)
			{
				// Spawn the AI controller and possess the enemy
				UWorld* World = GetWorld();
				AAIController* AIController = World->SpawnActor<AAIController>(EnemyAIControllerClass, LastSpawnedEnemy->GetActorLocation(), FRotator::ZeroRotator);

				if (AIController)
				{
					AIController->Possess(LastSpawnedEnemy);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Successfully spawned enemy at location: %s"), *SpawnLocation.ToString());
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy after %d attempts at spawner: %s"), MaxSpawnAttempts, *GetName());
}

TObjectPtr<AEnemyBase> AEnemySpawner::GetLastSpawnedEnemy() const
{
	return LastSpawnedEnemy;
}

void AEnemySpawner::SpawnEnemiesWithDelay(TSubclassOf<AEnemyBase> EnemyClass, float SpawnDelay)
{
	if (!EnemyClass) return;

	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([this, EnemyClass]()
	{
		SpawnEnemy(EnemyClass);
	}), SpawnDelay, false);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

