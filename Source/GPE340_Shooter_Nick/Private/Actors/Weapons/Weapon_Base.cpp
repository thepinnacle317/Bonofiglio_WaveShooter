// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Weapon_Base.h"
#include "Actors/Weapons/WeaponComp.h"

AWeapon_Base::AWeapon_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComp>(TEXT("Weapon Component"));
}

void AWeapon_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}
