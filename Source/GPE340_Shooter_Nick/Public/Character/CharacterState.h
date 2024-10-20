// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Firing UMETA(DisplayName = "Firing"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_Max UMETA(DisplayName = "Default Max")
};