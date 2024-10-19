// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterState.h"
#include "ShooterCharacterComp.generated.h"

/* Forward Declarations */
class ANick_ShooterCharacter;
class AWeapon_Base;

DECLARE_DELEGATE(FCrosshairDelegate);
DECLARE_DELEGATE(FWeaponActionDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GPE340_SHOOTER_NICK_API UShooterCharacterComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterCharacterComp();
	
	/* Used to set the trace range for the crosshairs */
	UPROPERTY(EditAnywhere, Category="Shooter Core | Camera Properties | Trace Properties")
	float CrosshairTraceLength = 50000.f;

	/* Method used for setting the value of aiming in the controller */
	void SetbIsAiming(bool Value);
	
	/* Used to set the transform of the weapon barrel socket. This is called in Nick_ShooterCharacter */
	void SetSocketTransform(const FTransform& Value);

	/* Used to set the value of the Current Camera FOV for when we are interping between values. */
	void SetCurrentCameraFOV(float Value);

	/* Used to set the sensitivity rates of the camera when aiming or not */
	void SetAimSensitivity();


	/* Move to the Weapon Component once the base weapon item has been setup with pickup logic */
	/* The Weapon Component will have delegates that can be called to execute logic based on input or events */
	void FirePressed();
	void FireReleased();

	/* Runs a trace from the center of the crosshairs outwards */
	void CrosshairTrace();

	/* Get the Socket Transform for the held Weapon */
	// TODO: Refactor for when the holdable weapon logic is done.  Will Get the socket from the mesh of the held actor.
	void SetWeaponSocketTransform();

	/* Drops the current held weapon and throws it in front of the character */
	void DropWeapon();

	/* Equips the weapon to the character by socket */
	void EquipWeapon(AWeapon_Base* WeaponToBeEquipped);

	/* Drops current weapon and equips the weapon under the interaction trace */
	void SwapWeapon(AWeapon_Base* WeaponForSwapping);

	void ReloadPressed();

	/* Used for retrieving generic data that is need to access the anim instance */
	TObjectPtr<ANick_ShooterCharacter> OwningCharacter;

	/* * * Delegate Handles * * */
	FCrosshairDelegate OnCrosshairTrace;
	FWeaponActionDelegate WeaponActionDelegate;

	/* Directional Dodge Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	/* Dive Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Animation")
	TObjectPtr<UAnimMontage> DiveMontage;

	/* Hipfire Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Animation")
	TObjectPtr<UAnimMontage> HipFireMontage;

	/* Bullet Trace particle effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | VFX")
	TObjectPtr<UParticleSystem> VaporTrail;

	/* Bullet Impact visual effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | VFX")
	TObjectPtr<UParticleSystem> ImpactFX;

	/* Field of View default for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Camera Properties | FOV")
	float DefaultCameraFOV;

	/* Field of View when the player is aiming */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Camera Properties | FOV")
	float AimingCameraFOV;
	
	/* Speed at which the FOV with smoothly transition between values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Camera Properties | FOV")
	float AimInterpSpeed;
	
	/* Default Movement Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Speed")
	float DefaultCharacterSpeed;

	/* Movement Speed When Aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Speed")
	float AimingCharacterSpeed;

	/* The Default Look Speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float BaseLookUpRate;

	/* The Default Turn Speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float BaseTurnRate;

	/* Out Of Combat Turn Speed  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float AtReadyTurnRate;

	/* Out Of Combat Look Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float AtReadyLookUpRate;

	/* Mouse/Controller Turning Sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float AimingTurnRate;

	/* Mouse/Controller Look Sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement | Aiming")
	float AimingLookUpRate;

	/* Used to determine the state of the character for performing actions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | States")
	ECharacterState CharacterState;



protected:
	virtual void BeginPlay() override;
	
	/* Spawns the starter weapon for the character at start */
	AWeapon_Base* SpawnDefaultWeapon();

private:
	/* Location of the bullet vapor particle endpoint */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | No Edit", meta = (AllowPrivateAccess = "true"))
	FVector VaporEndPoint;

	/* Used to toggle the aiming state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | No Edit", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	/* This is set in the character through SetWeaponSocketTransform for the barrelsocket location.
	 * TODO: Update once there is a pickup and item system. */
	FTransform SocketTransform;

	/* Current Frames Camer FOV */
	float CurrentCameraFOV;

	/* Used to determine if the fire button is being pressed */
	bool bFireButtonDown;

	/* Should a bullet/trace happen?  */
	bool bShouldFire;

	/* The Currently Equipped Weapon on the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | Weapons", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeapon_Base> CurrentWeapon;

	/* Used to set the default weapon the player will start with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooter Core", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon_Base> DefaultWeaponClass;

public:
	/* The compiler will ultimately determine if these getters will be Inline or not */
	FORCEINLINE FTransform GetSocketTransform() const { return SocketTransform; }
	FORCEINLINE bool GetbIsAiming() const { return bIsAiming; }
	FORCEINLINE float GetCurrentCameraFOV() const { return CurrentCameraFOV; }
	FORCEINLINE FVector GetVaporEndpoint() const { return VaporEndPoint; }
	FORCEINLINE void SetVaporEndpoint(const FVector& EndPoint) { VaporEndPoint = EndPoint; }
	FORCEINLINE bool GetbFireButtonDown() const { return bFireButtonDown; }
	FORCEINLINE bool GetbShouldFire() const { return bShouldFire; }
	FORCEINLINE void SetbShouldFire(bool ShouldFire) { bShouldFire = ShouldFire; }
	FORCEINLINE AWeapon_Base* GetCurrentWeapon() { return CurrentWeapon; }
		
};
