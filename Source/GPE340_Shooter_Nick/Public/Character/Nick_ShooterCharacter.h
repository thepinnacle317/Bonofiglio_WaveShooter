// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PickupInterface.h"
#include "Nick_ShooterCharacter.generated.h"

/* Forward Declarations */
class UShooterCharacterComp;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UInventoryComponent;
class UInteractionComponent;
class UAttributeComponent;
class ANick_ShooterPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAimDelegate);
//DECLARE_DELEGATE(FFireDelegate);
//DECLARE_DELEGATE(FOnPlayerDeath);
//DECLARE_DELEGATE(FAimDelegate);

UCLASS()
class GPE340_SHOOTER_NICK_API ANick_ShooterCharacter : public ACharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	
	ANick_ShooterCharacter();
	virtual void Tick(float DeltaTime) override;

	/* Delegate Handles */
	FFireDelegate OnFiredWeapon;
	FAimDelegate OnAiming;
	FOnPlayerDeath OnPlayerDeath;

	FVector GetTargetInterpLocation();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	void Aim();

	void InterpolateCameraFOV(float DeltaTime);

private:

	UFUNCTION()
	void HandlePlayerDeath();

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
