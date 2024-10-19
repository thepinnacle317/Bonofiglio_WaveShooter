// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InventoryComponent.h"
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
	AmmoMap.Add(EAmmoTypes::EA_Pistol, DefaultPistolAmmo);
	AmmoMap.Add(EAmmoTypes::EA_Rifle, DefaultRifleAmmo);
	AmmoMap.Add(EAmmoTypes::EA_Special, DefaultSpecialAmmo);
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
