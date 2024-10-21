// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InventoryComponent.h"

#include "Actors/Ammo/AmmoItem.h"
#include "Actors/Weapons/WeaponComp.h"
#include "Actors/Weapons/Weapon_Base.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"


UInventoryComponent::UInventoryComponent() :
/* Member Initializer List */
DefaultPistolAmmo(60),
DefaultRifleAmmo(180),
DefaultSpecialAmmo(6),
// Camera Interpolation of Item
CameraInterpForaward(250.f),
CameraInterpUp(65.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/* Character reference for the owner of the component *** Used mostly for getting skeletal data */
	OwningCharacter = Cast<ANick_ShooterCharacter>(GetOwner());

	/* Initialize Ammo Map */
	InitialzeAmmoMap();
}

void UInventoryComponent::InitialzeAmmoMap()
{
	/* Add the ammo type and the default value to the map for initialization */
	AmmoMap.Add(EAmmoTypes::EAT_PistolAmmo, DefaultPistolAmmo);
	AmmoMap.Add(EAmmoTypes::EAT_RifleAmmo, DefaultRifleAmmo);
	AmmoMap.Add(EAmmoTypes::EAT_SpecialAmmo, DefaultSpecialAmmo);
}

bool UInventoryComponent::HasAmmoInGun()
{
	// Check that there is a valid weapon
	if (OwningCharacter->GetShooterComp()->GetCurrentWeapon() == nullptr) return false;

	// Check that there is ammo in the gun
	return OwningCharacter->GetShooterComp()->GetCurrentWeapon()->GetWeaponComponent()->GetWeaponAmmo() > 0;
}

bool UInventoryComponent::HasAmmoType()
{
	if (OwningCharacter->GetShooterComp()->GetCurrentWeapon() == nullptr) return false;

	EAmmoTypes AmmoType = OwningCharacter->GetShooterComp()->GetCurrentWeapon()->GetWeaponComponent()->GetAmmoType();

	// This checks to see that there is ammo in the map of the type that our current equipped weapon uses
	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}

	return false;
}

void UInventoryComponent::GetPickupItem(AItem_Base* PickupItem)
{
	auto Weapon = Cast<AWeapon_Base>(PickupItem);
	if (Weapon)
	{
		OwningCharacter->GetShooterComp()->SwapWeapon(Weapon);
	}
}

void UInventoryComponent::PickupAmmo(AAmmoItem* AmmoItem)
{
	// See if the Ammo map has the ammo type that we are picking up
	if (AmmoMap.Find(AmmoItem->GetAmmoType()))
	{
		// Index the element in the map to get the amount in our ammo map
		int32 AmmoCount{ AmmoMap[AmmoItem->GetAmmoType()]};
		// Add the ammo to the player ammo inventory
		AmmoCount += AmmoItem->GetItemAmount();
		// Sets the amount of ammo in the map to the type
		AmmoMap[AmmoItem->GetAmmoType()] = AmmoCount;

		// Check to see if the current weapon the player is holding has the same ammo type as the ammo the player is picking up
		if (OwningCharacter->GetShooterComp()->GetCurrentWeapon()->GetWeaponComponent()->GetAmmoType() == AmmoItem->GetAmmoType())
		{
			// Check to see if the current equipped weapon has no ammo
			if (OwningCharacter->GetShooterComp()->GetCurrentWeapon()->GetWeaponComponent()->GetWeaponAmmo() == 0)
			{
				// Automatically Reload the weapon after picking up ammo
				OwningCharacter->GetShooterComp()->GetCurrentWeapon()->GetWeaponComponent()->ReloadWeapon();
			}
		}
	}
}
