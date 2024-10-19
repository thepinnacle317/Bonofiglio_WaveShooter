// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Ammo/AmmoItem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

AAmmoItem::AAmmoItem()
{
	/* Constructs the Ammo Mesh and sets it as the root component */
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Mesh"));
	SetRootComponent(AmmoMesh);

	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetCollisionBox()->SetupAttachment(GetRootComponent());
}

void AAmmoItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();
}
