// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

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

	/* The current Yaw of the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	 float CharacterYaw;

	/* Yaw of the character from the previos frame */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement | Turn In Place", meta = (AllowPrivateAccess = "true"))
	float LastFrameCharacterYaw;

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
};
