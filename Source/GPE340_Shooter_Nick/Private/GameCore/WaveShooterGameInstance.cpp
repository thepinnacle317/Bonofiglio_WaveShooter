// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/WaveShooterGameInstance.h"

UWaveShooterGameInstance::UWaveShooterGameInstance() :
CurrentWaveNumber(1)
{
	
}

void UWaveShooterGameInstance::IncrementWaveNumber()
{
	CurrentWaveNumber++;
}
