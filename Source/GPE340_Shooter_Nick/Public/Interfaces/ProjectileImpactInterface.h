// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileImpactInterface.generated.h"

UINTERFACE(MinimalAPI)
class UProjectileImpactInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GPE340_SHOOTER_NICK_API IProjectileImpactInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Used to define logic when something is hit by a Projectile */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ProjectileHit(FHitResult HitResult);
};
