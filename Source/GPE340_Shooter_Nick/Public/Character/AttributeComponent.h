// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPE340_SHOOTER_NICK_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	

private:
	/* Current Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes Properties | Health", meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/* Current Stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes Properties | Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

public:
	FORCEINLINE float GetHealth() const {return Health; }
	FORCEINLINE float GetStamina() const {return Stamina; }
	FORCEINLINE void SetHealth(float Value) { Health = Value; }
	FORCEINLINE void SetStamina(float Value) { Stamina = Value; }
	FORCEINLINE void SetMaxHealth(float Value) { MaxHealth = Value; }
	FORCEINLINE void SetMaxStamina(float Value) { MaxStamina = Value; }

		
};
