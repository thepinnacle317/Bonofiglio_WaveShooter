// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "Character/InventoryEnums.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"


/* Weapon Delegates */
DECLARE_DELEGATE(FWeaponTraceDelegate);

/* Forward Declarations */
class AWeapon_Base;
class ANick_ShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GPE340_SHOOTER_NICK_API UWeaponComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Runs a trace from the barrel socket to the end of the crosshairs trace */
	void WeaponTrace();

	/* Starts the fire timer for automatic fire */
	void StartFireTimer();
	/* Used to reset the fire timer if we stop or are still holding down the input */
	UFUNCTION()
	void FireTimerReset();

	/* Used to decrease the amount of ammo in the gun when firing */
	void DecrementAmmo();
	
	/* Weapon Delegates */
	FWeaponTraceDelegate WeaponTraceDelegate;

	/* Reference to the Characer */  // BAAAD!  This is a workaround for now
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties | Combat ")
	TObjectPtr<ANick_ShooterCharacter> OwningShooterCharacter;
	
	/* Used to set the rate of fire for the weapon when the fire input is held down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat | Weapons")
	float AutoFireRate;

	/* Designer variable for how far they want the weapon to fly in front of the character when dropped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat | Weapons")
	float ImpulseStrength;

	/* Should be scaled based on strength *** Can be used for more bounces or distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat Properties | Weapons")
	float WeaponThrowTime;

	/* Reference to the actor that owns the component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties | Combat ")
	TObjectPtr<AWeapon_Base> OwningActor;

	/* Used to jump to different sections of the monatage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat Properties | Animation")
	FName ReloadSection;

	/* Reload Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat Properties | Animation")
	TObjectPtr<UAnimMontage> ReloadMontage;

	/* Firing Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Combat Properties | Animation")
	TObjectPtr<UAnimMontage> FireMontage;
	
	/* Timer For weapon Firing */
	FTimerHandle AutoFireTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties | Ammunition")
	int32 HeldAmmo;

	/* Handles firing the weapon */
	void FireWeapon();

	/* Used for reloading the weapon */
	void ReloadWeapon();

	/* Applies and impulse to the weapon it the forward direction *** Called in DropWeapon() */
	void ThrowWeapon();

	void ReloadAmmoInMagazine(int32 Amount);

	void InitializeHeldAmmo();

	
	UFUNCTION(BlueprintCallable)
	void FinishedReloading();

protected:
	virtual void BeginPlay() override;

	/* Resets the falling variable and sets the item state to PickUpReady so that the player can retrieve it again */
	void StopFalling();

	bool MagIsFull();
	
	// TODO: Create a function and timer for how long the item will live in the world once it has been dropped.  When it expired destroy it.
	// Reset the timer if it has been picked up again.

private:
	/* Handle for controlling and manager the timer for the throw states */
	FTimerHandle ThrowWeaponHandle;
	
	bool bIsFalling;

	/* The amount of ammo in the gun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Ammunition", meta = (AllowPrivateAccess = "true"))
	int32 AmmoInGun;

	/* Plays the Sound associated with the weapon when firing */
	void PlayWeaponSound();

	/* Plays the weapon Fire Montage */
	void PlayFireMontage();

	bool bInitialized;

	/* Defines the archetype of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Weapon Archetype", meta = (AllowPrivateAccess = "true"))
	EWeaponTypes WeaponTypes;

	/* Defines the type of ammo that is used by the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Weapon Archetype", meta = (AllowPrivateAccess = "true"))
	EAmmoTypes AmmoType;

	/* Defines the maximum amount of bullets that can be loaded into the gun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties | Ammunition", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

public:
	FORCEINLINE void SetOwningShooterCharacter(ANick_ShooterCharacter* ShooterCharacter) { OwningShooterCharacter = ShooterCharacter; }
	FORCEINLINE int32 GetWeaponAmmo() const { return AmmoInGun; }
	FORCEINLINE EWeaponTypes GetWeaponType() const { return WeaponTypes; }
	FORCEINLINE EAmmoTypes GetAmmoType() const { return AmmoType; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	
};
