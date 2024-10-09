// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAmmoTypes: uint8
{
	EA_Pistol UMETA(DisplayName = "Pistol Ammo"),
	EA_Rifle UMETA(DisplayName = "Rifle Ammo"),
	EA_Special UMETA(DisplayName = "Special Ammo"),

	EA_Max UMETA(DisplayName = "Default Max")
};