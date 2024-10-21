// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OverlapItem.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AOverlapItem::AOverlapItem()
{

	/* Constructs the Ammo Mesh and sets it as the root component */
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	SetRootComponent(PickupMesh);
	
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Sphere"));
	PickupSphere->SetupAttachment(GetRootComponent());
	
	GetCollisionBox()->SetupAttachment(GetItemMesh());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	
}

void AOverlapItem::SetItemProps(EItemState State)
{
	Super::SetItemProps(State);
	
	switch (State)
	{
	case EItemState::EIS_PickupReady:
		/// * * Item Props * * ///
		/* The item does not need physics while waiting to be picked up */
		PickupMesh->SetSimulatePhysics(false);
		PickupMesh->SetEnableGravity(false);
		PickupMesh->SetVisibility(true);
		/* The mesh should not be using collision for interaction *** Uses the Interaction Box */
		PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Equipped:
		/* The item does not need physics while waiting to be picked up */
		PickupMesh->SetSimulatePhysics(false);
		PickupMesh->SetEnableGravity(false);
		PickupMesh->SetVisibility(true);
		/* The mesh should not be using collision for interaction *** Uses the Interaction Box */
		PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Falling:
		/// * * Collision & Physics Props * * ///
		PickupMesh->SetSimulatePhysics(true);
		PickupMesh->SetEnableGravity(true);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PickupMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		break;
		
	case EItemState::EIS_Equipping:
		/* The item does not need physics while waiting to be picked up */
		PickupMesh->SetSimulatePhysics(false);
		PickupMesh->SetEnableGravity(false);
		PickupMesh->SetVisibility(true);
		/* The mesh should not be using collision for interaction *** Uses the Interaction Box */
		PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}
