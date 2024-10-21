// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Ammo/AmmoItem.h"
#include "Character/InventoryComponent.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"

AAmmoItem::AAmmoItem() :
/* Member Initializer List */
AmmoType(EAmmoTypes::EAT_Max)
{
	// True so we can interp the item location effect
	PrimaryActorTick.bCanEverTick = true;
}

void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();
	GetPickupShpere()->OnComponentBeginOverlap.AddDynamic(this, &AAmmoItem::OnSphereOverlap);
	GetPickupShpere()->OnComponentBeginOverlap.AddDynamic(this, &AAmmoItem::OnSphereEndOverlap);
}

void AAmmoItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
		ANick_ShooterCharacter* PickupCharacter = Cast<ANick_ShooterCharacter>(OtherActor);
		if (PickupCharacter)
		{
			PickupCharacter->GetInventoryComp()->PickupAmmo(this);
		}
		PlayPickupSound();
		PlayPickupEffects();
		
		Destroy();
	}
}

void AAmmoItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}
