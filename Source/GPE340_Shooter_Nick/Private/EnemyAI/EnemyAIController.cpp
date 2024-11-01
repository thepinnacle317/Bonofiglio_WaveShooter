// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	check(Blackboard);
	EnemyBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");
	check(EnemyBehaviorTree);
}
