// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacterComp.h"
#include "Actors/Weapons/WeaponComp.h"
#include "Actors/Weapons/Weapon_Base.h"
#include "Character/AttributeComponent.h"
#include "Character/InteractionComponent.h"
#include "Character/InventoryComponent.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameCore/Nick_ShooterPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/ProjectileImpactInterface.h"
#include "Kismet/GameplayStatics.h"

UShooterCharacterComp::UShooterCharacterComp() :
/* Member Initializer List : Put these in order by initialization */
	// Camera FOV Values
	DefaultCameraFOV(90.f),
	AimingCameraFOV(40.f),
	AimInterpSpeed(20.f),
	// Character Speed Values
	DefaultCharacterSpeed(500.f),
	AimingCharacterSpeed(350),
	bIsAiming(false),
	CurrentCameraFOV(0.f),
	// Aim Sensitivity
	BaseLookUpRate(45.f),
	BaseTurnRate(45.f),
	AtReadyTurnRate(90.f),
	AtReadyLookUpRate(90.f),
	AimingTurnRate(25.f),
	AimingLookUpRate(25.f),
	// Gun Input Variables
	bFireButtonDown(false),
	bShouldFire(true),
	CharacterState(ECharacterState::ECS_Unoccupied),
	SprintSpeed(1000.f),
	bIsSprinting(false)
	
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UShooterCharacterComp::BeginPlay()
{
	Super::BeginPlay();

	/* Character reference for the owner of the component *** Used mostly for getting skeletal data */
	OwningCharacter = Cast<ANick_ShooterCharacter>(GetOwner());

	/* Crosshair Trace delegate binding */
	OnCrosshairTrace.BindUObject(this, &UShooterCharacterComp::CrosshairTrace);

	/* Base setting of the Camera FOV */
	CurrentCameraFOV = DefaultCameraFOV;
	
}

void UShooterCharacterComp::CrosshairTrace()
{
	// Get the current size of the viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get the center of the screen.
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	
	CrosshairLocation.Y -= 50.f; // Move the crosshair up 50 units on the screen.
	
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Transforms the given 2D screen space coordinate into a 3D world-space point and direction.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetOwner(), 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	// Was the deprojection successful
	if (bScreenToWorld)
	{
		const FVector Start{CrosshairWorldPosition};
		const FVector End{CrosshairWorldPosition + CrosshairWorldDirection * CrosshairTraceLength};

		// Perform Line Trace From Crosshair.
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECC_Visibility);
		
		/* Spawn Impact Particles */
		if (ScreenTraceHit.bBlockingHit)
		{
			if (ScreenTraceHit.GetActor()->IsValidLowLevel())
			{
				// Get projectile interface from hit actor
				IProjectileImpactInterface* ProjectileImpactInterface = Cast<IProjectileImpactInterface>(ScreenTraceHit.GetActor());
				if (ProjectileImpactInterface)
				{
					ProjectileImpactInterface->ProjectileHit_Implementation(ScreenTraceHit);
				}
				else
				{
					/*** Spawn the impact particles where the blocking hit was ***/
					if (ImpactFX)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX,
							ScreenTraceHit.Location);
					}
				}
				if (CurrentWeapon)
				{
					/*** Have the weapon deal damage to the Hit Actor ***/
					CurrentWeapon->GetWeaponComponent()->DealDamage(ScreenTraceHit);
				}
			}
		}
		if (CurrentWeapon != nullptr)
		{
			// Call WeaponTrace Delegate
			CurrentWeapon->WeaponComponent->WeaponTraceDelegate.Execute();
		}
	}
}

AWeapon_Base* UShooterCharacterComp::SpawnDefaultWeapon()
{
	/* Make sure that the weapon class was set in the character and is valid */
	if (DefaultWeaponClass)
	{
		/* Set the local WeaponActor to the spawn class, so it can be attached */
		return GetWorld()->SpawnActor<AWeapon_Base>(DefaultWeaponClass);
	}
	return nullptr;
}

void UShooterCharacterComp::StartSprinting()
{
	/* Make sure the player has stamina left to use */
	if (OwningCharacter->GetAttributeComp()->GetStamina() > 0.f)
	{
		/* Set sprinting to true and change the characters movement speed */
		bIsSprinting = true;
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void UShooterCharacterComp::StopSprinting()
{
	/* Set sprinting to false and return the player to default movement speed when not sprinting */
	bIsSprinting = false;
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultCharacterSpeed;
}

void UShooterCharacterComp::EquipWeapon(AWeapon_Base* WeaponToBeEquipped)
{
	if (WeaponToBeEquipped)
	{
		/* Get the socket that the weapon will be attached to */
		const USkeletalMeshSocket* WeaponSocket = OwningCharacter->GetMesh()->GetSocketByName(FName("WeaponSocket_R"));

		if (WeaponSocket)
		{
			/* Attach the weapon to the weapon socket on the right hand */
			WeaponSocket->AttachActor(WeaponToBeEquipped, OwningCharacter->GetMesh());
		}

		/* Sets the Current Weapon to the weapon that was spawned */
		CurrentWeapon = WeaponToBeEquipped;

		/* This is a hard reference for the weapon and its component */
		// This needs to be cleaned up when swapping, dropping, and destroying as well.
		CurrentWeapon->OwningCharacter = OwningCharacter;
		CurrentWeapon->WeaponComponent->OwningShooterCharacter = OwningCharacter;

		// Bind Delegates when the weapon is equipped
		OwningCharacter->GetShooterController()->FiringDelegate.BindUObject(CurrentWeapon->GetWeaponComponent(), &UWeaponComp::FireWeapon);
		OwningCharacter->GetShooterController()->ReloadDelegate.BindUObject(CurrentWeapon->GetWeaponComponent(), &UWeaponComp::ReloadWeapon);
		
		/* Change the state of the Item/Weapon to equipped*/
		/* Set Collision Box to ignore all collision channels, so we do not hit it or trigger the widget Based on the state*/
		CurrentWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void UShooterCharacterComp::SwapWeapon(AWeapon_Base* WeaponForSwapping)
{
	OwningCharacter->GetInventoryComp()->SwitchWeapons(WeaponForSwapping);
	DropWeapon();
	EquipWeapon(WeaponForSwapping);
	OwningCharacter->GetInteractComp()->HitItem = nullptr;
	OwningCharacter->GetInteractComp()->HitItemLastFrame = nullptr;
}

void UShooterCharacterComp::ReloadPressed()
{
	OwningCharacter->GetShooterController()->ReloadDelegate.Execute();
}

void UShooterCharacterComp::DropWeapon()
{
	if (CurrentWeapon)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld,true);
		CurrentWeapon->GetItemMesh()->DetachFromComponent(DetachRules);

		/* Set the Item State when we drop the weapon so that the collision and physics are handled */
		CurrentWeapon->SetItemState(EItemState::EIS_Falling);
		CurrentWeapon->WeaponComponent->ThrowWeapon();
	}
	
	/* Set the Weapon to null since we no longer have possession of the weapon */
	CurrentWeapon = nullptr;
}

void UShooterCharacterComp::FirePressed()
{
	bFireButtonDown = true;

	if (CurrentWeapon != nullptr)
	{
		OwningCharacter->GetShooterController()->FiringDelegate.Execute();
	}
}

void UShooterCharacterComp::FireReleased()
{
	bFireButtonDown = false;
}

void UShooterCharacterComp::SetAimSensitivity()
{
	if (bIsAiming)
	{
		/* Set the camera to a lower sensitivity with the higher camera zoom */
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		/* Set the rates to higher sensitivity when the camera is not zoomed in */
		BaseTurnRate = AtReadyTurnRate;
		BaseLookUpRate = AtReadyLookUpRate;
	}
}

void UShooterCharacterComp::SetbIsAiming(bool Value)
{
	bIsAiming = Value;
}

void UShooterCharacterComp::SetSocketTransform(const FTransform& Value)
{
	SocketTransform = Value;
}

void UShooterCharacterComp::SetCurrentCameraFOV(float Value)
{
	CurrentCameraFOV = Value;
}

void UShooterCharacterComp::SetWeaponSocketTransform()
{
	if (CurrentWeapon == nullptr) return;
	
	const USkeletalMeshSocket* BarrelSocket = CurrentWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		SetSocketTransform(BarrelSocket->GetSocketTransform(CurrentWeapon->GetItemMesh()));
	}
}

