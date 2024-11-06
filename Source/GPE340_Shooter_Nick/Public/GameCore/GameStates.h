// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EGameStates: uint8
{
	EGS_MainMenu UMETA(DisplayName = "Main Menu"),
	EGS_InGame UMETA(DisplayName = "In Game"),
	EGS_PauseGame UMETA(DisplayName = "Pause Game"),
	EGS_GameOver UMETA(DisplayName = "Game Over"),
	EGS_RestartGame UMETA(DisplayName = "Restart Game"),
	
	EGS_Max UMETA(DisplayName = "Default Max")
};