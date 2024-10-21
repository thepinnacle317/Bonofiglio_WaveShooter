// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/OverlapItem.h"
#include "Character/InventoryEnums.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AAmmoItem : public AOverlapItem
{
	GENERATED_BODY()

public:
	AAmmoItem();
	/* Overlap Functions */
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void BeginPlay() override;

	

private:
	/* Classifies the ammo type for the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo", meta = (AllowPrivateAccess = "true"))
	EAmmoTypes AmmoType;

public:

	FORCEINLINE EAmmoTypes GetAmmoType() const { return AmmoType; }
	
};
