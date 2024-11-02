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

	UFUNCTION(BlueprintNativeEvent, Category="Enemy Spawner")
	void SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass);

	TObjectPtr<AEnemyBase> GetLastSpawnedEnemy() const;

protected:
	virtual void BeginPlay() override;

private:
	/* Reference used to track the last spawned enemy */
	UPROPERTY()
	TObjectPtr<AEnemyBase> LastSpawnedEnemy;

};
