// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Nick_ShooterPlayerController.generated.h"

/* Weapon Delegates */
DECLARE_DELEGATE(FWeaponTraceDelegate);
DECLARE_DELEGATE(FFiringDelegate);
DECLARE_DELEGATE(FReloadDelegate);
DECLARE_DELEGATE(FThrowWeaponDelegate);

/* Forward Declarations */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ANick_ShooterCharacter;

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API ANick_ShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	float Forward_Backward;
	
	UPROPERTY(BlueprintReadOnly)
	float Left_Right;

	FWeaponTraceDelegate WeaponTraceDelegate;
	FFiringDelegate FiringDelegate;
	FReloadDelegate ReloadDelegate;
	FThrowWeaponDelegate ThrowWeaponDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Possessed Pawn")
	TObjectPtr<ANick_ShooterCharacter> PossessedCharacter;

private:
	
	/* Enhanced Input */
	UPROPERTY(EditAnywhere, Category="Input | Context")
	TObjectPtr<UInputMappingContext> ShooterIMC;
	UPROPERTY(EditAnywhere, Category="Input | Movement")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input | Movement")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category="Input | Movement")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> DropItemAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> CycleWeaponAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category="Input | Actions")
	TObjectPtr<UInputAction> PauseMenuAction;

	/* Movement */
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void MoveCompleted();
	void JumpStarted();
	void JumpEnd();
	void Dodge();
	FName GetDirectionalDodgeSection();
	void SprintingInputPressed();
	void SprintingInputReleased();

	/* Gameplay Actions */
	void Firing();
	void NotFiring();
	void AimStarted();
	void AimCompleted();
	void Interact();
	void DropHeldItem();
	void ReloadWeapon();
	void PauseGame();

	/* Main Hud Overlay Class *** Set in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Properties | Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainOverlayClass;

	/* Variable for the Main Overlay Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HUD Properties | Widgets", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> MainOverlay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="HUD Properties | Widgets", meta = (AllowPrivateAccess = "true"))
	bool PauseToggled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD Properties | Widgets", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> PauseWidget;
};
