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
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

