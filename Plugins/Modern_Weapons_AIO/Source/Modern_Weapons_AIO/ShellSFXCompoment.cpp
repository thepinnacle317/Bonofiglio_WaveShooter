// Copyright SOUNDFX STUDIO © 2023

#include "ShellSFXCompoment.h"
#include "Sound/SoundCue.h"


// Sets default values for this component's properties
UShellSFXCompoment::UShellSFXCompoment()
{
}


// Called when the game starts
void UShellSFXCompoment::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UShellSFXCompoment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UShellSFXCompoment::Play_AmmunitionCasings(FVector location, float hitValue, EPhysicalSurface surfaceType)
{
	USoundFunctionLibrary::PlaySoundAtLocation(this, CasingsSoundsSurface[surfaceType].Cue_AmmunitionCasings, location, hitValue);
}


// FOR MULTIPLAYER
void UShellSFXCompoment::Play_AmmunitionCasings_ForMultiplayer(FVector location, float hitValue, EPhysicalSurface surfaceType)
{
	if (GetOwner() == nullptr)
		return;

	Play_AmmunitionCasings(location, hitValue, surfaceType);

	if (!GetOwner()->HasAuthority())
		Server_Play_AmmunitionCasings(location, hitValue, surfaceType);
}

void UShellSFXCompoment::Server_Play_AmmunitionCasings_Implementation(FVector location, float hitValue, EPhysicalSurface surfaceType)
{
	Multi_Play_AmmunitionCasings(location, hitValue, surfaceType);
}
bool UShellSFXCompoment::Server_Play_AmmunitionCasings_Validate(FVector location, float hitValue, EPhysicalSurface surfaceType) { return true; }

void UShellSFXCompoment::Multi_Play_AmmunitionCasings_Implementation(FVector location, float hitValue, EPhysicalSurface surfaceType)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_AmmunitionCasings(location, hitValue, surfaceType);
	}
}
bool UShellSFXCompoment::Multi_Play_AmmunitionCasings_Validate(FVector location, float hitValue, EPhysicalSurface surfaceType) { return true; }