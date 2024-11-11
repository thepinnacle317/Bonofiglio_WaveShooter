// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttributeComponent.h"

#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterAnimInstance.h"
#include "Character/ShooterCharacterComp.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent() :
/* Member Initializer List */
CurrentHealth(100.f),
MaxHealth(100.f),
CurrentStamina(100.f),
MaxStamina(100.f),
StaminaRegenRate(5.f),
StaminaDrainRate(3.f)
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentStamina = MaxStamina;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacter = Cast<ANick_ShooterCharacter>(GetOwner());
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStamina(DeltaTime);
}

void UAttributeComponent::UpdateStamina(float DeltaTime)
{
	if (OwningCharacter)
	{
		UShooterAnimInstance* ShooterAnimInstance = Cast<UShooterAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance());
		
		// Check to make sure the player is sprinting and that they have stamina to use for sprinting.  Also make sure they have the appropriate speed.
		if (ShooterAnimInstance && ShooterAnimInstance->GetSpeed() > 0.f && OwningCharacter->GetShooterComp()->bIsSprinting && CurrentStamina > 0.f)
		{
			// Drain stamina
			CurrentStamina -= StaminaDrainRate * DeltaTime;
			if (CurrentStamina <= 0.f)
			{
				// Set stamina to 0 and stop sprinting if they have run out
				CurrentStamina = 0.f;
				OwningCharacter->GetShooterComp()->StopSprinting();
			}
		}
		else if (!OwningCharacter->GetShooterComp()->bIsSprinting && CurrentStamina < MaxStamina)
		{
			// Regenerate stamina if the player is not currently sprinting
			CurrentStamina += StaminaRegenRate * DeltaTime;
			// Set the stamina to max stamina if it has been completely regenerated
			if (CurrentStamina > MaxStamina)
			{
				CurrentStamina = MaxStamina;
			}
		}
	}
	
}

