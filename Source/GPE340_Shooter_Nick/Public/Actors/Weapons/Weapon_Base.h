// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item_Base.h"
#include "Weapon_Base.generated.h"


/* Forward Declarations */
class UWeaponComp;
class ANick_ShooterCharacter;

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API AWeapon_Base : public AItem_Base
{
	GENERATED_BODY()

public:
	AWeapon_Base();

	virtual void Tick(float DeltaSeconds) override;

	/* Weapon Component holds all the core logic for the weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Core")
	TObjectPtr<UWeaponComp> WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Core | Properties")
	TObjectPtr<ANick_ShooterCharacter> OwningCharacter;
	
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE TObjectPtr<UWeaponComp> GetWeaponComponent() const { return WeaponComponent; }

};
