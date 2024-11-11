// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


class ANick_ShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPE340_SHOOTER_NICK_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateStamina(float DeltaTime);
	
protected:
	virtual void BeginPlay() override;
	

private:
	/* Current Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes Properties | Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	/* Max Health possible for the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/* Current Stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float CurrentStamina;
	
	/* Max Stamina the player has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	/* How fast stamina will recover when not sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRegenRate;

	/* How fast stamina will drain while the player is sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainRate;

	UPROPERTY()
	TObjectPtr<ANick_ShooterCharacter> OwningCharacter;	

public:
	FORCEINLINE float GetHealth() const {return CurrentHealth; }
	FORCEINLINE float GetStamina() const {return CurrentStamina; }
	FORCEINLINE void SetHealth(float Value) { CurrentHealth = Value; }
	FORCEINLINE void SetStamina(float Value) { CurrentStamina = Value; }
	FORCEINLINE void SetMaxHealth(float Value) { MaxHealth = Value; }
	FORCEINLINE void SetMaxStamina(float Value) { MaxStamina = Value; }

		
};
