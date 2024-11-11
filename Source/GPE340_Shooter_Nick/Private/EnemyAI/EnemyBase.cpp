// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AttributeComponent.h"
#include "EnemyAI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"


AEnemyBase::AEnemyBase() :
CritHitBone("head")
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AEnemyBase::HandleEnemyDeath()
{
	if (OnDeath.IsBound())
	{
		// Broadcast the delegate so the listener in the wave manager can track that it will need to spawn more if available from the count.
		OnDeath.Broadcast();
	}
	
	// Spawn Death particles.
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation());

	// Drop Scrap loot actor
	
	// TODO: Handle other things to happen when the enemy dies

	Destroy();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::ProjectileHit_Implementation(FHitResult HitResult)
{
	if (ImpactSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation());
	}
	if (ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, HitResult.Location, FRotator(0.f), true);
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (GetAttributeComp())
	{
		if (GetAttributeComp()->GetHealth() - DamageAmount <= 0.f)
		{
			// Could be swapped for a clamp
			GetAttributeComp()->SetHealth(0.f);
			
			// Kill the enemy.  This will also destroy the actor after all effects have been played.
			HandleEnemyDeath();
		}
		else
		{
			GetAttributeComp()->SetHealth(GetAttributeComp()->GetHealth() - DamageAmount);
		}
	}
	
	return DamageAmount;
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EnemyAIController = Cast<AEnemyAIController>(NewController);

	 if (EnemyBehaviorTree)
	 {
	 	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*EnemyBehaviorTree->BlackboardAsset);
	 	EnemyAIController->RunBehaviorTree(EnemyBehaviorTree);
	 }
}

