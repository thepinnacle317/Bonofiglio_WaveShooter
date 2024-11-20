// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	UFUNCTION(BlueprintNativeEvent, Category="Enemy Spawner | Functions")
	void SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass);

	TObjectPtr<AEnemyBase> GetLastSpawnedEnemy() const;

	// Maximum number of attempts to find a valid spawn location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Spawner | Spawn Properties")
	float SpawnRadius = 100.0f;
	
	// Radius around the initial location to attempt alternate spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Spawner | Spawn Properties")
	int32 MaxSpawnAttempts = 5;

	void SpawnEnemiesWithDelay(TSubclassOf<AEnemyBase> EnemyClass, float SpawnDelay);

protected:
	virtual void BeginPlay() override;

private:
	
	/* Reference used to track the last spawned enemy */
	UPROPERTY()
	TObjectPtr<AEnemyBase> LastSpawnedEnemy;

};
