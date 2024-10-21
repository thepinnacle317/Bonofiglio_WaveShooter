// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EWeaponClassification : uint8
{
	EWC_WraithSniper UMETA(DisplayName = "Wraith Sniper"),
	EWC_SciFiAR1 UMETA(DisplayName = "Sci-Fi AR-1"),
	EWC_SciFiAR2 UMETA(DisplayName = "Sci-Fi AR-2"),
	EWC_SciFiPistol UMETA(DisplayName = "Sci-Fi Pistol"),

	EWC_Max UMETA(DisplayName = "Default Max")
};