// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EAmmoTypes: uint8
{
	EAT_PistolAmmo UMETA(DisplayName = "Pistol Ammo"),
	EAT_RifleAmmo UMETA(DisplayName = "Rifle Ammo"),
	EAT_SpecialAmmo UMETA(DisplayName = "Special Ammo"),

	EAT_Max UMETA(DisplayName = "Default Max")
};