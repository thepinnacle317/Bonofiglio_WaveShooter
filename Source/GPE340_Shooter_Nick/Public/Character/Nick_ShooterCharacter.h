// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Nick_ShooterCharacter.generated.h"

class ANick_ShooterPlayerController;
/* Forward Declarations */
class UShooterCharacterComp;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UInventoryComponent;
class UInteractionComponent;

DECLARE_DELEGATE(FFireDelegate);
DECLARE_DELEGATE(FAimDelegate);

UCLASS()
class GPE340_SHOOTER_NICK_API ANick_ShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANick_ShooterCharacter();
	virtual void Tick(float DeltaTime) override;

	/* Delegate Handles */
	FFireDelegate OnFiredWeapon;
	FAimDelegate OnAiming;

protected:
	
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	void Aim();

	void InterpolateCameraFOV(float DeltaTime);

private:

	/* Camera boom used to position the camera behind the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/* Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/* Component that holds essential functionality to character gameplay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UShooterCharacterComp> ShooterCharacterComp;

	/* Component that holds Interaction functionality */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractionComponent> InteractionComponent;

	/* Component that holds Interaction functionality */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ANick_ShooterPlayerController> OwningController;
	

public:

	/* Returns Camera Boom Subobject */
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }

	/* Returns Follow Camera Subobject */
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }

	/* Returns the Shooter Component */
	FORCEINLINE TObjectPtr<UShooterCharacterComp> GetShooterComp() const { return ShooterCharacterComp; }

	/* Returns the Interaction Component */
	FORCEINLINE TObjectPtr<UInteractionComponent> GetInteractComp() const { return InteractionComponent; }

	/* Returns the Inventory Component */
	FORCEINLINE TObjectPtr<UInventoryComponent> GetInventoryComp() const { return InventoryComponent; }

	/* Returns the owning shooter player controller */
	FORCEINLINE TObjectPtr<ANick_ShooterPlayerController> GetShooterController() const { return OwningController; }

};
