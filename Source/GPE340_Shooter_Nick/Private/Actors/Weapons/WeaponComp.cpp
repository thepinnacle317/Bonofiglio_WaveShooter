// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponComp.h"
#include "Actors/Weapons/WeaponTypes.h"
#include "Actors/Weapons/Weapon_Base.h"
#include "Character/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"
#include "EnemyAI/EnemyBase.h"


UWeaponComp::UWeaponComp() :
/* Member Initializer List */
AutoFireRate(0.2f),
ImpulseStrength(10000.f),
WeaponThrowTime(.4f),
bIsFalling(false),
AmmoInGun(0),
WeaponTypes(EWeaponTypes::EWT_Rifle),
AmmoType(EAmmoTypes::EAT_RifleAmmo),
MagazineCapacity(30),
HeldAmmo(0),
bInitialized(false),
bIsAutomatic(true)

{
	/* Must be true for this component */
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	/* Get the weapon that owns this component */
	OwningActor = Cast<AWeapon_Base>(GetOwner());
		
	/* Bind the Fire Function */
	WeaponTraceDelegate.BindUObject(this, &UWeaponComp::WeaponTrace);

	bInitialized = false;

}

void UWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* Keeps the weapon Upright *** This is used for handling the world widget orientation */
	if (OwningActor->GetItemState() == EItemState::EIS_Falling && bIsFalling)
	{
		const FRotator MeshRotation{0.f, OwningActor->GetItemMesh()->GetComponentRotation().Yaw, 0.f};
		OwningActor->GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	
	if (bInitialized == false)
	{
		if (OwningShooterCharacter && OwningShooterCharacter->GetInventoryComp())
		{
			InitializeHeldAmmo();
			bInitialized = true;
		}
	}
}

void UWeaponComp::StopFalling()
{
	/* Set the bool since it will stop falling based on the timer and make it ready to be picked up */
	bIsFalling = false;
	OwningActor->SetItemState(EItemState::EIS_PickupReady);
}

bool UWeaponComp::MagIsFull()
{
	return AmmoInGun >= MagazineCapacity;
}

void UWeaponComp::PlayWeaponSound()
{
	if (WeaponSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSFX, GetOwner()->GetActorLocation(), FRotator(0.f));
	}
}

void UWeaponComp::SpawnMuzzleFlash()
{
	if (WeaponMuzzleVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(WeaponMuzzleVFX, OwningActor->GetItemMesh(), MuzzleSocket);
	}
}

void UWeaponComp::PlayFireMontage()
{
	/* Play montage associated with firing weapons */
	UAnimInstance* AnimInstance = OwningShooterCharacter->GetMesh()->GetAnimInstance();
		
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
		
		/* Execute Crosshair Line Trace */
		OwningShooterCharacter->GetShooterComp()->OnCrosshairTrace.Execute();
	}
}

void UWeaponComp::InitializeHeldAmmo()
{
	// Set ammo held to the type that matches the weapon in the inventory ammo map
	HeldAmmo = OwningShooterCharacter->GetInventoryComp()->AmmoMap[AmmoType];
}

void UWeaponComp::DealDamage(FHitResult HitResult)
{
	AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitResult.GetActor());
	if (HitEnemy)
	{
		// Crit Hit Damage
		if (HitResult.BoneName.ToString() == HitEnemy->GetCritHitBone())
		{
			UGameplayStatics::ApplyDamage(HitEnemy,
			GetCritHitDamage(),
			OwningShooterCharacter->GetController(),
			OwningShooterCharacter,
			UDamageType::StaticClass());
		}
		// Normal Weapon Damage
		else
		{
			UGameplayStatics::ApplyDamage(HitEnemy,
			GetWeaponDamage(),
			OwningShooterCharacter->GetController(),
			OwningShooterCharacter,
			UDamageType::StaticClass());
		}
	}
}

/** Perform a trace from the weapon's muzzle to the
 * end hit location of the crosshair to check for obstructing objects.
 */
void UWeaponComp::WeaponTrace()
{
	/*** Line Trace from Gun Barrel ***/
	FHitResult WeaponHitResults;
	const FVector WeaponTraceStart{ OwningShooterCharacter->GetShooterComp()->GetSocketTransform().GetLocation() };
	const FVector WeaponTraceEnd{ OwningShooterCharacter->GetShooterComp()->GetVaporEndpoint() };
	GetWorld()->LineTraceSingleByChannel(WeaponHitResults, WeaponTraceStart, WeaponTraceEnd, ECC_Visibility);

	// Check for hit between the weapons muzzle and the VaporEndPoint
	if (WeaponHitResults.bBlockingHit)
	{
		// TODO:Spawn emitter for hit between 
	}
}

void UWeaponComp::StartFireTimer()
{
	if (OwningShooterCharacter == nullptr) return;

	// Set the character state to firing 
	OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Firing;

	// Start the firing timer
	GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &UWeaponComp::FireTimerReset, AutoFireRate, false);
}

void UWeaponComp::FireTimerReset()
{
	if (OwningShooterCharacter == nullptr) return;

	// Reset the character state from firing 
	OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Unoccupied;
	
	// Check to make sure the gun has ammo
	if (OwningShooterCharacter->GetInventoryComp()->HasAmmoInGun())
	{
		// Are we holding the button down
		//OwningShooterCharacter->GetShooterComp()->SetbShouldFire(true);
		if (OwningShooterCharacter->GetShooterComp()->GetbFireButtonDown())
		{
			FireWeapon();
		}
	}
	else
	{
		ReloadWeapon();
	}
}

void UWeaponComp::DecrementAmmo()
{
	// Check if the ammo value is negative then set it to 0
	if (AmmoInGun - 1 <= 0)
	{
		AmmoInGun = 0;
	}
	else
	{
		// Decrement the ammo if we have some
		--AmmoInGun;
	}
}

/*************************************** CALLABLE ACTIONS **************************************/

void UWeaponComp::FireWeapon()
{
	// Check to make sure that there is a valid owning character for the weapon.
	if (OwningShooterCharacter == nullptr) return;

	// If we are not in unoccupied do not start firing.
	if (OwningShooterCharacter->GetShooterComp()->CharacterState != ECharacterState::ECS_Unoccupied) return;
	
	// Set the weapon socket transform
	OwningShooterCharacter->GetShooterComp()->SetWeaponSocketTransform();

	if (OwningShooterCharacter->GetInventoryComp()->HasAmmoInGun())
	{
		if (bIsAutomatic)
		{
			PlayWeaponSound();
			SpawnMuzzleFlash();
			/* Play the weapon firing montage */
			PlayFireMontage();
			/* Subtract one from the ammo each time it is fired */
			DecrementAmmo();
			StartFireTimer();
		}
		/* Semi Auto */
		else
		{
			// Set the character state to firing 
			OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Firing;
			PlayWeaponSound();
			SpawnMuzzleFlash();
			/* Play the weapon firing montage */
			PlayFireMontage();
			/* Subtract one from the ammo each time it is fired */
			DecrementAmmo();
			// Reset the character state from firing 
			OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Unoccupied;

			if (!OwningShooterCharacter->GetInventoryComp()->HasAmmoInGun()) ReloadWeapon();
		}
	}
}

void UWeaponComp::ReloadWeapon()
{
	if (OwningShooterCharacter->GetShooterComp()->CharacterState != ECharacterState::ECS_Unoccupied) return;

	// Is there ammo of the correct type in the ammo map that we can reload from
	if (OwningShooterCharacter->GetInventoryComp()->HasAmmoType() && !MagIsFull())
	{
		// Set the character state to reloading
		OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Reloading;
		
		/* Play montage associated with firing weapons */
		UAnimInstance* AnimInstance = OwningShooterCharacter->GetMesh()->GetAnimInstance();
		
		if (AnimInstance && ReloadMontage)
		{
			/* The montage will be specific to each weapon.  This allows for various different animations that are weapon
			 * specific.
			 * For future this will allow for situational based reload animations to be played by jumping the section.
			 */
			AnimInstance->Montage_Play(ReloadMontage);
			
			AnimInstance->Montage_JumpToSection(ReloadSection);
		}
	}
}

void UWeaponComp::ThrowWeapon()
{
	/* Initialize rotation to the item meshes yaw rotation */
	FRotator MeshRotation{0.f, OwningActor->GetItemMesh()->GetComponentRotation().Yaw, 0.f};
	OwningActor->GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	/* Get Mesh Forward Vector */
	const FVector MeshForward{ OwningActor->GetItemMesh()->GetForwardVector() };

	/* Get Mesh Right Vector */
	const FVector MeshRight{ OwningActor->GetItemMesh()->GetRightVector() };
	
	/* Direction to throw the weapon *** -20 degress from the meshes forward vector when called. */
	FVector MeshImpulse = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	/* Give Random Rotation */
	float RandomMeshRotation{FMath::FRandRange(5.f, 15.f)};
	MeshImpulse = MeshRight.RotateAngleAxis(RandomMeshRotation, FVector(0.f,0.f, 1.f));

	/* Impulse Strength */
	MeshImpulse *= 10000.f;
	OwningActor->GetItemMesh()->AddImpulse(MeshImpulse);

	/* Set the bool to falling & start the timer for when it is in the air */
	bIsFalling = true;
	GetWorld()->GetTimerManager().SetTimer(ThrowWeaponHandle, this, &UWeaponComp::StopFalling, WeaponThrowTime);
}

void UWeaponComp::ReloadAmmoInMagazine(int32 Amount)
{
	// Assert to check if we are trying to load more than allowed
	//checkf(AmmoInGun + Amount <= MagazineCapacity, TEXT("Attempted to reload the weapon with more ammo than magazine can hold"));
	AmmoInGun += Amount;
}

void UWeaponComp::FinishedReloading()
{
	TObjectPtr<UInventoryComponent> InventoryComponent = OwningShooterCharacter->GetInventoryComp();
	
	/* Reset the Character State, this allows us to perform other actions once we have completed reloading */
	OwningShooterCharacter->GetShooterComp()->CharacterState = ECharacterState::ECS_Unoccupied;

	// Check to see that there is ammo of the type that the weapon needs in the map
	if (OwningShooterCharacter->GetInventoryComp()->AmmoMap.Contains(AmmoType))
	{
		// Checks to see how much room is left in the magazine for reloading
		const int32 RoomInMagazine = GetMagazineCapacity() - AmmoInGun;

		if (RoomInMagazine > HeldAmmo)
		{
			// Reload the magazine with all held ammo
			ReloadAmmoInMagazine(HeldAmmo);
			HeldAmmo = 0;
			// Update the ammo map with a new key value pair
			InventoryComponent->AmmoMap.Add(AmmoType, HeldAmmo);
		}
		else
		{
			// Fill the magazine
			ReloadAmmoInMagazine(RoomInMagazine);
			HeldAmmo -= RoomInMagazine;
			InventoryComponent->AmmoMap.Add(AmmoType, HeldAmmo);
		}
	}
}

