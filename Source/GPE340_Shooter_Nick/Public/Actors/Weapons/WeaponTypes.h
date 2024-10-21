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
