// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Nick_ShooterCharacter.generated.h"

class UAttributeComponent;
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

	const FVector GetTargetInterpLocation() const;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interpolation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ItemInterpTargetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeComponent> AttributeComponent;
	

public:
	
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE TObjectPtr<UShooterCharacterComp> GetShooterComp() const { return ShooterCharacterComp; }
	FORCEINLINE TObjectPtr<UInteractionComponent> GetInteractComp() const { return InteractionComponent; }
	FORCEINLINE TObjectPtr<UInventoryComponent> GetInventoryComp() const { return InventoryComponent; }
	FORCEINLINE TObjectPtr<ANick_ShooterPlayerController> GetShooterController() const { return OwningController; }
	FORCEINLINE TObjectPtr<USceneComponent> GetItemInterpTargetComp() const { return ItemInterpTargetComp; }
	FORCEINLINE TObjectPtr<UAttributeComponent> GetAttributeComp() const { return  AttributeComponent; }

};
