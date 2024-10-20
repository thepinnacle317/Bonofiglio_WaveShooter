// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	EPT_Ammo UMETA(DisplayName = "Ammo"),
	EPT_Weapon UMETA(DisplayName = "Weapon"),
	EPT_Scrap UMETA(DisplayName = "Scrap"),

	EPT_Max UMETA(DisplayName = "Default Max")
};