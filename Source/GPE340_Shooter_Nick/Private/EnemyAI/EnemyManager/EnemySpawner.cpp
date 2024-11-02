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
	if (EnemyClass)
	{
		// Set up the spawn parameters 
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		
		// Spawn the enemy
		LastSpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters);

		if (LastSpawnedEnemy)
		{
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
			
			// This is left here to optionally add enemy behavior upon spawn.  Not Implemented.
			//LastSpawnedEnemy->ActivateEnemy();
		}
	}
}

TObjectPtr<AEnemyBase> AEnemySpawner::GetLastSpawnedEnemy() const
{
	return LastSpawnedEnemy;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

