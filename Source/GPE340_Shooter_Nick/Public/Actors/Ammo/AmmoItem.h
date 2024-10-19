// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item_Base.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AAmmoItem : public AItem_Base
{
	GENERATED_BODY()

public:
	AAmmoItem();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

	/* Mesh the will be visible for ammo pickups in the world */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> AmmoMesh;

public:
	/* Public Getters and Setters */
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetAmmoMesh() const { return AmmoMesh; }
	
};
