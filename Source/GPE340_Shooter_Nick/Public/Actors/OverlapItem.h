// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item_Base.h"
#include "OverlapItem.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AOverlapItem : public AItem_Base
{
	GENERATED_BODY()

public:
	AOverlapItem();

protected:
	virtual void SetItemProps(EItemState State) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> PickupSphere;

	/* Mesh the will be visible for pickups in the world */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PickupMesh;

public:
	/* Public Getters and Setters */
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetPickupMesh() const { return PickupMesh; }
	FORCEINLINE TObjectPtr<USphereComponent> GetPickupShpere() const { return PickupSphere; }
	
};
