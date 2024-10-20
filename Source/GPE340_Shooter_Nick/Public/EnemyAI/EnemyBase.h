// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ProjectileImpactInterface.h"
#include "EnemyBase.generated.h"

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyBase : public ACharacter, public IProjectileImpactInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	void ProjectileHit_Implementation(FHitResult HitResult) override;

protected:
	virtual void BeginPlay() override;

	/* Particle spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ImpactVFX;

	/* Sound spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSFX;

};
