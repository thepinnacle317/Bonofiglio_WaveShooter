// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor;

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> EnemyBehaviorTree;
	
	
};
