// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ProjectileImpactInterface.h"
#include "EnemyBase.generated.h"

/* Forward Declarations */
class AEnemyAIController;
class UBehaviorTree;
class UAttributeComponent;
class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS()
class GPE340_SHOOTER_NICK_API AEnemyBase : public ACharacter, public IProjectileImpactInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	virtual void ProjectileHit_Implementation(FHitResult HitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	// Event broadcast when the enemy dies
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDeathDelegate OnDeath;

	// Use a clear and unique name for the AI controller class
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AAIController> EnemyAIControllerClass;
	
protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	/* Particle spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ImpactVFX;

	/* Sound spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSFX;

	/* Sound spawned when hit by the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> CritHitSFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components")
	TObjectPtr<AEnemyAIController> EnemyAIController;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeComponent> AttributeComponent;

	/* Name of the bone to apply critical hit damage to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	FString CritHitBone;

	UPROPERTY(EditAnywhere, Category="Core Components")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;

	// Used to handle the enemy when they have no more health left
	void HandleEnemyDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> DeathParticles;

	/* Time to display health bar when damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Properties | Combat", meta = (AllowPrivateAccess = "true"))
	float HealthBarDisplayTime;
	
	FTimerHandle HealthBarTimerHandle;

public:
	FORCEINLINE TObjectPtr<UAttributeComponent> GetAttributeComp() const { return AttributeComponent; }
	FORCEINLINE FString GetCritHitBone() const { return CritHitBone; }

};
