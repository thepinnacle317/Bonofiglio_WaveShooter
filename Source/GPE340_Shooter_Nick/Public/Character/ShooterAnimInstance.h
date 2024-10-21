// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/WeaponClassification.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"


UENUM(BlueprintType)
enum EAOStates : uint8
{
	EAO_Aiming UMETA(DisplayName = "Aiming"),
	EAO_AtReady UMETA(DisplayName = "At Ready"),
	EAO_Reloading UMETA(DisplayName = "Reloading"),
	EAO_IsInAir UMETA(DisplayName = "Is In Air"),

	EAO_Max UMETA(DisplayName = "Default Max")
};
/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UShooterAnimInstance();

	/* */
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	
	virtual void NativeInitializeAnimation() override;

protected:
	/* Core Turn In Place function for calculating the associated variables */
	void TurnInPlace();

	/* Used to calculate the values required for leaning during locomotion */
	void Lean(float DeltaTime);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	class ANick_ShooterCharacter* ShooterCharacter;

	/* Used to get the speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	/* Used to check if the character is currently in the air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/* Used to determine if the character is moving or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Yaw;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Pitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Roll;

	/* Used to get the offset Yaw of the character to be applied to strafing*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float ShooterMovementOffsetYaw;

	/* Used to get the Last Frame Offset Yaw of the character to be applied to strafing*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float LastFrameOffsetYaw;

	/* The Yaw of the character if standing still and not in the air */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	 float TIPYaw;

	/* The Yaw of the character if standing still and not in the air */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	float LastFrameTIPYaw;

	/* The offset to be applied to the root bone when rotating the character *** This is a difference amount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	/* Current Rotation Curve Value */
	float RotationCurveValue;
	
	/* Last Frame Rotation Curve Value */
	float LastFrameRotationCurveValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	/* Used to determine the type of offset that the character will use during animation and gameplay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAOStates> AOStates;

	/* Current character yaw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Lean", meta = (AllowPrivateAccess = "true"))
	FRotator CharacterRotation;

	/* Character yaw from the previous frame */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Lean", meta = (AllowPrivateAccess = "true"))
	FRotator LastFrameCharacterRotation;

	/* Used for leaning with the blendspace while running */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement | Lean", meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	/* Used to define the classification of the equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat | Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponClassification WeaponClassification;
};
