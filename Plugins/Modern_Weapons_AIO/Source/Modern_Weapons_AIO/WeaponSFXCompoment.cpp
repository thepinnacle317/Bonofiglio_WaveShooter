// Copyright SOUNDFX STUDIO © 2023

#include "WeaponSFXCompoment.h"
#include "Sound/SoundCue.h"


// Sets default values for this component's properties
UWeaponSFXCompoment::UWeaponSFXCompoment()
{
}


// Called when the game starts
void UWeaponSFXCompoment::BeginPlay()
{
	Super::BeginPlay();

	InitialiseAllSounds();
}


// Called every frame
void UWeaponSFXCompoment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// Initialise All Sounds
void UWeaponSFXCompoment::InitialiseAllSounds()
{
	acs_Loop = CreateACs(cues_Loop);
	acs_Loop_Suppressor = CreateACs(cues_Loop_Suppressor);
}


// Play OneShot
void UWeaponSFXCompoment::Play_OneShot()
{
	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_OneShot, GetOwner()->GetActorLocation());
	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Stop, GetOwner()->GetActorLocation());
	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Tail, GetOwner()->GetActorLocation());
}

// Play OneShot and Loop Firing Sounds
void UWeaponSFXCompoment::Play_OneShotLoop(float crossfadeOneShotLoopTime)
{
	float offsetLoopTime = 0.f;
	if (offsetsLoopTime.Num() > 0)
		offsetLoopTime = offsetsLoopTime[FMath::RandRange(0, offsetsLoopTime.Num() - 1)];

	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_OneShot, GetOwner()->GetActorLocation());
	USoundFunctionLibrary::FadeInSounds(this, acs_Loop, crossfadeOneShotLoopTime, EAudioFaderCurve::Linear, offsetLoopTime - offsetOneShotTime);
	bIsLoopPlaying = true;
}

// Stop Loop Firing
void UWeaponSFXCompoment::Stop_Loop(float fadeOutDuration)
{
	if (bIsLoopPlaying)
	{
		USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Stop, GetOwner()->GetActorLocation());
		USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Tail, GetOwner()->GetActorLocation());
		USoundFunctionLibrary::FadeOutSounds(this, acs_Loop, fadeOutDuration);
		bIsLoopPlaying = false;
	}
}

// Play Suppressor One Shot Sounds
void UWeaponSFXCompoment::Play_OneShot_Suppressor()
{
	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_OneShot_Suppressor, GetOwner()->GetActorLocation());
	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Stop_Suppressor, GetOwner()->GetActorLocation());
}

// Play Suppressor OneShot and Loop Firing Sounds
void UWeaponSFXCompoment::Play_OneShotLoop_Suppressor(float crossfadeOneShotLoopTime)
{
	float offsetLoopTime = 0.f;
	if (offsetsLoopTime.Num() > 0)
		offsetLoopTime = offsetsLoopTime[FMath::RandRange(0, offsetsLoopTime.Num() - 1)];

	USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_OneShot_Suppressor, GetOwner()->GetActorLocation());
	USoundFunctionLibrary::FadeInSounds(this, acs_Loop_Suppressor, crossfadeOneShotLoopTime, EAudioFaderCurve::Linear, offsetLoopTime - offsetOneShotTime);
	bIsLoopPlaying = true;
}

// Stop Suppressor Loop Firing
void UWeaponSFXCompoment::Stop_Loop_Suppressor(float fadeOutDuration)
{
	if (bIsLoopPlaying)
	{
		USoundFunctionLibrary::PlaySoundsAtLocation(this, cues_Stop_Suppressor, GetOwner()->GetActorLocation());
		USoundFunctionLibrary::FadeOutSounds(this, acs_Loop_Suppressor, fadeOutDuration);
		bIsLoopPlaying = false;
	}
}

// Play Switch Fire Mode Sound
void UWeaponSFXCompoment::Play_SwitchFireMode()
{
	USoundFunctionLibrary::PlayAttachedShortDistanceSounds(this, cues_SwitchFireMode, GetOwner()->GetRootComponent());
}

// Play Equip/Unequip Suppressor Sound
void UWeaponSFXCompoment::Play_EquipUnequipSuppressor()
{
	USoundFunctionLibrary::PlayAttachedShortDistanceSounds(this, cues_EquipUnequipSuppressor, GetOwner()->GetRootComponent());
}


// Create Audio Component
UAudioComponent* UWeaponSFXCompoment::CreateAC(USoundCue * soundCue)
{
	if (soundCue)
	{
		UAudioComponent* ac = NewObject<UAudioComponent>(this, *soundCue->GetName());
		USoundFunctionLibrary::CreateAudioComponent(soundCue, GetOwner(), ac);
		return ac;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty SoundCue"));
		return nullptr;
	}
}

// Create Audio Components
TArray<UAudioComponent*> UWeaponSFXCompoment::CreateACs(TArray<USoundCue*> soundCues)
{
	TArray<UAudioComponent*> acs;
	for (USoundCue* soundCue : soundCues)
	{
		acs.Add(CreateAC(soundCue));
	}
	return acs;
}

// FOR MULTIPLAYER
// 
void UWeaponSFXCompoment::Play_OneShot_ForMultiplayer()
{
	if (GetOwner() == nullptr)
		return;

	Play_OneShot();

	if (!GetOwner()->HasAuthority())
		Server_Play_OneShot();
}

void UWeaponSFXCompoment::Play_OneShotLoop_ForMultiplayer(float crossfadeOneShotLoopTime)
{
	if (GetOwner() == nullptr)
		return;

	Play_OneShotLoop(crossfadeOneShotLoopTime);

	if (!GetOwner()->HasAuthority())
		Server_Play_OneShotLoop(crossfadeOneShotLoopTime);
}

void UWeaponSFXCompoment::Stop_Loop_ForMultiplayer(float fadeOutDuration)
{
	if (GetOwner() == nullptr)
		return;

	Stop_Loop(fadeOutDuration);

	if (!GetOwner()->HasAuthority())
		Server_Stop_Loop(fadeOutDuration);
}

void UWeaponSFXCompoment::Play_OneShot_Suppressor_ForMultiplayer()
{
	if (GetOwner() == nullptr)
		return;

	Play_OneShot_Suppressor();

	if (!GetOwner()->HasAuthority())
		Server_Play_OneShot_Suppressor();
}

void UWeaponSFXCompoment::Play_OneShotLoop_Suppressor_ForMultiplayer(float crossfadeOneShotLoopTime)
{
	if (GetOwner() == nullptr)
		return;

	Play_OneShotLoop_Suppressor(crossfadeOneShotLoopTime);

	if (!GetOwner()->HasAuthority())
		Server_Play_OneShotLoop_Suppressor(crossfadeOneShotLoopTime);
}

void UWeaponSFXCompoment::Stop_Loop_Suppressor_ForMultiplayer(float fadeOutDuration)
{
	if (GetOwner() == nullptr)
		return;

	Stop_Loop_Suppressor(fadeOutDuration);

	if (!GetOwner()->HasAuthority())
		Server_Stop_Loop_Suppressor(fadeOutDuration);
}

void UWeaponSFXCompoment::Play_SwitchFireMode_ForMultiplayer()
{
	if (GetOwner() == nullptr)
		return;

	Play_SwitchFireMode();

	if (!GetOwner()->HasAuthority())
		Server_Play_SwitchFireMode();
}

void UWeaponSFXCompoment::Play_EquipUnequipSuppressor_ForMultiplayer()
{
	if (GetOwner() == nullptr)
		return;

	Play_EquipUnequipSuppressor();

	if (!GetOwner()->HasAuthority())
		Server_Play_EquipUnequipSuppressor();
}
// 
// Play One Shot Sounds
void UWeaponSFXCompoment::Server_Play_OneShot_Implementation()
{
	Multi_Play_OneShot();
}
bool UWeaponSFXCompoment::Server_Play_OneShot_Validate() { return true; }

void UWeaponSFXCompoment::Multi_Play_OneShot_Implementation()
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_OneShot();
	}
}
bool UWeaponSFXCompoment::Multi_Play_OneShot_Validate() { return true; }

void UWeaponSFXCompoment::Server_Play_OneShotLoop_Implementation(float crossfadeOneShotLoopTime)
{
	Multi_Play_OneShotLoop(crossfadeOneShotLoopTime);
}
bool UWeaponSFXCompoment::Server_Play_OneShotLoop_Validate(float crossfadeOneShotLoopTime) { return true; }

void UWeaponSFXCompoment::Multi_Play_OneShotLoop_Implementation(float crossfadeOneShotLoopTime)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_OneShotLoop(crossfadeOneShotLoopTime);
	}
}
bool UWeaponSFXCompoment::Multi_Play_OneShotLoop_Validate(float crossfadeOneShotLoopTime) { return true; }

void UWeaponSFXCompoment::Server_Stop_Loop_Implementation(float fadeOutDuration)
{
	Multi_Stop_Loop(fadeOutDuration);
}
bool UWeaponSFXCompoment::Server_Stop_Loop_Validate(float fadeOutDuration) { return true; }

void UWeaponSFXCompoment::Multi_Stop_Loop_Implementation(float fadeOutDuration)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Stop_Loop(fadeOutDuration);
	}
}
bool UWeaponSFXCompoment::Multi_Stop_Loop_Validate(float fadeOutDuration) {	return true; }

// Suppressed Functions
// 
void UWeaponSFXCompoment::Server_Play_OneShot_Suppressor_Implementation()
{
	Multi_Play_OneShot_Suppressor();
}
bool UWeaponSFXCompoment::Server_Play_OneShot_Suppressor_Validate() { return true; }

void UWeaponSFXCompoment::Multi_Play_OneShot_Suppressor_Implementation()
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_OneShot_Suppressor();
	}
}
bool UWeaponSFXCompoment::Multi_Play_OneShot_Suppressor_Validate() { return true; }

void UWeaponSFXCompoment::Server_Play_OneShotLoop_Suppressor_Implementation(float crossfadeOneShotLoopTime)
{
	Multi_Play_OneShotLoop_Suppressor(crossfadeOneShotLoopTime);
}
bool UWeaponSFXCompoment::Server_Play_OneShotLoop_Suppressor_Validate(float crossfadeOneShotLoopTime) {	return true; }

void UWeaponSFXCompoment::Multi_Play_OneShotLoop_Suppressor_Implementation(float crossfadeOneShotLoopTime)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_OneShotLoop_Suppressor(crossfadeOneShotLoopTime);
	}
}
bool UWeaponSFXCompoment::Multi_Play_OneShotLoop_Suppressor_Validate(float crossfadeOneShotLoopTime) { return true; }

void UWeaponSFXCompoment::Server_Stop_Loop_Suppressor_Implementation(float fadeOutDuration)
{
	Multi_Stop_Loop_Suppressor(fadeOutDuration);
}
bool UWeaponSFXCompoment::Server_Stop_Loop_Suppressor_Validate(float fadeOutDuration) { return true; }

void UWeaponSFXCompoment::Multi_Stop_Loop_Suppressor_Implementation(float fadeOutDuration)
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Stop_Loop_Suppressor(fadeOutDuration);
	}
}
bool UWeaponSFXCompoment::Multi_Stop_Loop_Suppressor_Validate(float fadeOutDuration) { return true; }

void UWeaponSFXCompoment::Server_Play_SwitchFireMode_Implementation()
{
	Multi_Play_SwitchFireMode();
}
bool UWeaponSFXCompoment::Server_Play_SwitchFireMode_Validate() { return true; }

void UWeaponSFXCompoment::Multi_Play_SwitchFireMode_Implementation()
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_SwitchFireMode();
	}
}
bool UWeaponSFXCompoment::Multi_Play_SwitchFireMode_Validate() { return true; }

void UWeaponSFXCompoment::Server_Play_EquipUnequipSuppressor_Implementation()
{
	Multi_Play_EquipUnequipSuppressor();
}
bool UWeaponSFXCompoment::Server_Play_EquipUnequipSuppressor_Validate() { return true; }

void UWeaponSFXCompoment::Multi_Play_EquipUnequipSuppressor_Implementation()
{
	if (GetOwner()->GetLocalRole() < ROLE_AutonomousProxy)
	{
		Play_EquipUnequipSuppressor();
	}
}
bool UWeaponSFXCompoment::Multi_Play_EquipUnequipSuppressor_Validate() { return true; }