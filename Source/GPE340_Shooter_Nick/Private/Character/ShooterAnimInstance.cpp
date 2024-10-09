// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterAnimInstance.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UShooterAnimInstance::UShooterAnimInstance():
/* Member Initializer List */
ShooterCharacter(nullptr),
Speed(0.f),
bIsInAir(false),
bIsMoving(false),
bAiming(false),
Yaw(0.f),
Pitch(0.f),
Roll(0.f),
ShooterMovementOffsetYaw(0.f),
LastFrameOffsetYaw(0.f),
CharacterYaw(0.f),
LastFrameCharacterYaw(0.f),
RootYawOffset(0.f),
AimPitch(0.f),
bReloading(false)
{
}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	// Redundant try to get a reference to the character in case if fails.
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<ANick_ShooterCharacter>(TryGetPawnOwner());
	}
	if (ShooterCharacter)
	{
		if (ShooterCharacter->GetShooterComp()->CharacterState == ECharacterState::ECS_Reloading)
		{
			bReloading = true;
		}
		else
		{
			bReloading = false;
		}
		
		// Get the lateral speed of the character from their velocity
		FVector Velocity{ ShooterCharacter->GetVelocity() };
		// Zero Z so we only get the lateral velocity
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// Check if the character is in the air
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		// Check if the character is accelerating					Check the magnitude of the vector retrieved from the movement Comp.
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsMoving = true;
		}
		else
		{
			bIsMoving = false;
		}

		FRotator ShooterAimRotation = ShooterCharacter->GetBaseAimRotation();
		FRotator ShooterMovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());

		/* Returns a Rotator and we only need the Yaw */
		ShooterMovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(ShooterMovementRotation, ShooterAimRotation).Yaw;

		/* If the character is moving then update the last frame offset yaw to determine the delta so we play
		 * the right stop animation */
		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastFrameOffsetYaw = ShooterMovementOffsetYaw;
		}

		bAiming = ShooterCharacter->GetShooterComp()->GetbIsAiming();

	}
	/* Update the variables need to execute Turn In Place */
	TurnInPlace();
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ShooterCharacter = Cast<ANick_ShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace()
{
	if (ShooterCharacter == nullptr) return;
	
	AimPitch = ShooterCharacter->GetBaseAimRotation().Pitch;
	
	if (Speed > 0)
	{
		/* No Turning in Place. Reset the values */
		RootYawOffset = 0.f;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		LastFrameCharacterYaw = CharacterYaw;
		RotationCurveValue = 0.f;
		LastFrameRotationCurveValue = 0.f;
	}
	else
	{
		LastFrameCharacterYaw = CharacterYaw;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float YawDelta{ CharacterYaw - LastFrameCharacterYaw };

		/* Clamped and updated RootYawOffset -180 +180  *** See Comment for function */
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);

		/* Within the anim instance we can access a curve value on an animation through the function with the
		 * TEXT Macro to specify the curve that we want to retrieve the value from */
		const float Turning{ GetCurveValue(TEXT("Turning"))};
		/* 1 if TIP (Turn In Place) and 0 if not TIP */
		if (Turning > 0)
		{
			LastFrameRotationCurveValue = RotationCurveValue;
			RotationCurveValue = GetCurveValue(TEXT("Rotation"));
			
			/* Initialize Delta Rotation */
			const float DeltaRotation{ RotationCurveValue - LastFrameRotationCurveValue};

			/* Check the value of RootYawOffset to determine the direction *** > 0 -> Left + , < 0 -> Right - */
								// Turning Left					// Turning Right
			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			/* Check to see if we over rotated */
			const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
			if (ABSRootYawOffset > 90)
			{
				/* If we have then find the excess rotation amount */
				const float RootYawExcess{ ABSRootYawOffset - 90.f };
				/* Move the root bone the excess amount to closer match our final target rotation */
				RootYawOffset > 0 ? RootYawOffset -= RootYawExcess : RootYawOffset += RootYawExcess;
			}
		}

		/*
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, -1, FColor::Emerald,
				FString::Printf(TEXT("Character Yaw: %f"), CharacterYaw));
			GEngine->AddOnScreenDebugMessage(2, -1, FColor::Cyan,
				FString::Printf(TEXT("Root Yaw Offset: %f"), RootYawOffset));
		}
		*/
	}
}
