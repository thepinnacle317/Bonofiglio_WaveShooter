// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Weapon_Base.h"
#include "Actors/Weapons/WeaponComp.h"

AWeapon_Base::AWeapon_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComp>(TEXT("Weapon Component"));
}

void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}
