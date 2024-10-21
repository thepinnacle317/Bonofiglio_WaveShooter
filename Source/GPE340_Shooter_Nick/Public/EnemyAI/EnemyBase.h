// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ProjectileImpactInterface.h"
#include "EnemyBase.generated.h"

class UAttributeComponent;

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyBase : public ACharacter, public IProjectileImpactInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	virtual void ProjectileHit_Implementation(FHitResult HitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	/* Particle spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ImpactVFX;

	/* Sound spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSFX;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeComponent> AttributeComponent;

	/* Name of the bone to apply critical hit damage to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	FString CritHitBone;

public:
	FORCEINLINE TObjectPtr<UAttributeComponent> GetAttributeComp() const { return AttributeComponent; }
	FORCEINLINE FString GetCritHitBone() const { return CritHitBone; }

};
