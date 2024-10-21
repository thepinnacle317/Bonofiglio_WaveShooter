// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
	EWT_Rifle UMETA(DisplayName = "Rifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_Special UMETA(DisplayName = "Special"),

	EWT_Max UMETA(DisplayName = "Default Max")
};

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	EWC_WraithSniper UMETA(DisplayName = "Wraith Sniper"),
	EWC_SciFiAR1 UMETA(DisplayName = "Sci-Fi AR-1"),
	EWC_SciFiAR2 UMETA(DisplayName = "Sci-Fi AR-2"),
	EWC_SciFiPistol UMETA(DisplayName = "Sci-Fi Pistol"),

	EWC_Max UMETA(DisplayName = "Default Max")
};