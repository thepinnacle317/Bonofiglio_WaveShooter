// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent() :
/* Member Initializer List */
Health(100.f),
MaxHealth(100.f),
Stamina(100.f),
MaxStamina(100.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::Die()
{
	// TODO:Play electrical explosion
	// TODO:Ragdoll Enemy
	// TODO:Hide Health bar
	// TODO:Disable Collision
	// TODO:Drop Scrap
	GetOwner()->Destroy();
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

