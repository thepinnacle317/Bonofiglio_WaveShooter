// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/Nick_ShooterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Weapons/WeaponComp.h"
#include "Actors/Weapons/Weapon_Base.h"
#include "Blueprint/UserWidget.h"
#include "Character/InteractionComponent.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"
#include "GameFramework/CharacterMovementComponent.h"


void ANick_ShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	 // Set the input mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ShooterIMC, 0);
	}

	// Make sure the variable was set in blueprints
	if (MainOverlayClass)
	{
		// Set the main overlay
		MainOverlay = CreateWidget<UUserWidget>(this, MainOverlayClass);
		if (MainOverlay)
		{
			// Add the widget to the viewport and set it to visible
			MainOverlay->AddToViewport();
			MainOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

/* Holds the bindings to all the player inputs & their action values/methods */
void ANick_ShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//UE_LOG(LogTemp, Warning, TEXT("Input Component Setup."));
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted);
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpStarted);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::JumpEnd);
	ShooterInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ThisClass::Firing);
	ShooterInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ThisClass::NotFiring);
	ShooterInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::AimStarted);
	ShooterInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimCompleted);
	ShooterInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ThisClass::Dodge);
	ShooterInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted);
	ShooterInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &ThisClass::DropHeldItem);
	ShooterInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
	ShooterInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::ReloadWeapon);

}

void ANick_ShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedCharacter = Cast<ANick_ShooterCharacter>(InPawn);
}

void ANick_ShooterPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	PossessedCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	Forward_Backward = MovementVector.Y;
	PossessedCharacter->AddMovementInput(RightDirection, MovementVector.X);
	Left_Right = MovementVector.X;
}

void ANick_ShooterPlayerController::MoveCompleted()
{
	Forward_Backward = 0.f;
	Left_Right = 0.f;
}

void ANick_ShooterPlayerController::Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	/* Add yaw and pitch input to controller *** Use Delta Seconds and base rates for smoother input, especially when zoomed in */
	PossessedCharacter->AddControllerPitchInput(LookAxisVector.Y * PossessedCharacter->GetShooterComp()->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	PossessedCharacter->AddControllerYawInput(LookAxisVector.X * PossessedCharacter->GetShooterComp()->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANick_ShooterPlayerController::JumpStarted()
{
	if (PossessedCharacter != nullptr)
	{
		PossessedCharacter->Jump();
	}
}

void ANick_ShooterPlayerController::JumpEnd()
{
	if (PossessedCharacter != nullptr)
	{
		PossessedCharacter->StopJumping();
	}
}

void ANick_ShooterPlayerController::Dodge()
{
	/* Get the anim instance from the possesesed character so we can play a montage */
	UAnimInstance* AnimInstance = PossessedCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* SelectedDodge;
		if (PossessedCharacter->GetCharacterMovement()->bOrientRotationToMovement)
		{
			SelectedDodge = PossessedCharacter->GetShooterComp()->DiveMontage;				
		}
		else
		{
			SelectedDodge = PossessedCharacter->GetShooterComp()->DodgeMontage;
		}
		
		AnimInstance->Montage_Play(SelectedDodge);
		// Jump to the section of the montage that will be determined by input value and switch to the correct case.
		if (!PossessedCharacter->GetCharacterMovement()->bOrientRotationToMovement)
		{
			AnimInstance->Montage_JumpToSection(GetDirectionalDodgeSection());
		}
	}
}

FName ANick_ShooterPlayerController::GetDirectionalDodgeSection()
{
	FName MontageSection;
	int32 SelectIndex = FMath::TruncToInt(Forward_Backward * 2.f + Left_Right) + 2;
	switch (SelectIndex)
	{
	case 0:
		MontageSection = FName("BWD");
		break;
	case 1:
		MontageSection = FName("LEFT");
		break;
	case 2:
		MontageSection = FName("FWD");
		break;
	case 3:
		MontageSection = FName("RIGHT");
		break;
	case 4:
		MontageSection = FName("FWD");
		break;
	}
	return MontageSection;
}

void ANick_ShooterPlayerController::Firing()
{
	PossessedCharacter->GetShooterComp()->FirePressed();
}

void ANick_ShooterPlayerController::NotFiring()
{
	PossessedCharacter->GetShooterComp()->FireReleased();
}

void ANick_ShooterPlayerController::AimStarted()
{
	/* Set Aim Value to True */
	PossessedCharacter->GetShooterComp()->SetbIsAiming(true);

	/* Set Character Walk Speed While Aiming */
	PossessedCharacter->GetCharacterMovement()->MaxWalkSpeed = PossessedCharacter->GetShooterComp()->AimingCharacterSpeed;

	/* Delegate is used here to make a call so that we handle all the animation and camera work in the
	 * character method and not pollute the controller */
	PossessedCharacter->OnAiming.Execute();
	
	//UE_LOG(LogTemp, Warning, TEXT("Aiming"));
}

void ANick_ShooterPlayerController::AimCompleted()
{
	PossessedCharacter->GetShooterComp()->SetbIsAiming(false);

	/* Set Character Walk Speed While Aiming */
	PossessedCharacter->GetCharacterMovement()->MaxWalkSpeed = PossessedCharacter->GetShooterComp()->DefaultCharacterSpeed;
}

void ANick_ShooterPlayerController::Interact()
{
	if (PossessedCharacter->GetShooterComp()->GetCharacterState() != ECharacterState::ECS_Unoccupied) return;

	if (PossessedCharacter && PossessedCharacter->GetInventoryComp() && PossessedCharacter->GetInteractComp()->HitItem)
	{
		PossessedCharacter->GetInteractComp()->HitItem->StartItemCurve(PossessedCharacter);
		// Keeps from spamming the item curve animation
		PossessedCharacter->GetInteractComp()->HitItem = nullptr;

		/*  ** Used to test the swap weapon logic **
		auto HitWeapon = Cast<AWeapon_Base>(PossessedCharacter->GetInteractComp()->HitItem);
		PossessedCharacter->GetShooterComp()->SwapWeapon(HitWeapon); */
	}
}

void ANick_ShooterPlayerController::DropHeldItem()
{
	PossessedCharacter->GetShooterComp()->DropWeapon();
}

void ANick_ShooterPlayerController::ReloadWeapon()
{
	PossessedCharacter->GetShooterComp()->ReloadPressed();
}
