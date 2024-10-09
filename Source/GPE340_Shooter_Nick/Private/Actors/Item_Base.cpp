// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item_Base.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


AItem_Base::AItem_Base():
ItemName(FString("Default Weapon")),
ItemAmount(0),
ItemRarity(EItemRarity::EIR_Common),
ItemState(EItemState::EIS_PickupReady)
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Widget"));
	PickupWidget->SetupAttachment(GetRootComponent());

}

void AItem_Base::BeginPlay()
{
	Super::BeginPlay();

	/* Hide Pickup Widget At Start */
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	/* Visual Set the value of the item rarity */
	SetItemRarity();

	/* Set the properties of the item based on the current state it is in */
	SetItemProps(ItemState);
}

void AItem_Base::SetItemRarity()
{
	/* Does not use zero so that the index aligns with the visual rarity value */
	for (int32 i = 0; i <= 5; i++)
	{
		ActiveRarity.Add(false);
	}

	switch (ItemRarity)
	{
	case EItemRarity::EIR_Common:
		ActiveRarity[1] = true;
		break;
	case EItemRarity::EIR_Uncommon:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		break;
	case EItemRarity::EIR_Rare:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		break;
	case EItemRarity::EIR_Epic:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		ActiveRarity[4] = true;
		break;
	case EItemRarity::EIR_Legendary:
		ActiveRarity[1] = true;
		ActiveRarity[2] = true;
		ActiveRarity[3] = true;
		ActiveRarity[4] = true;
		ActiveRarity[5] = true;
		break;
	}
}

void AItem_Base::SetItemProps(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_PickupReady:
		/// * * Item Props * * ///
		/* The item does not need physics while waiting to be picked up */
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		/* The mesh should not be using collision for interaction *** Uses the Interaction Box */
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		/// * * Collision & Physics Props * * ///
		/* Clear all collision settings for the Collision Box */
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		/* Set it to Block the Visibility Channel */
		CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		/* Collsion Enabled to query for raycasts and simulate rigid bodies/constraints */
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		break;

	case EItemState::EIS_Equipped:
		/* Item Props */
		  
		/* Set Widget Visibility */
		PickupWidget->SetVisibility(false);
		
		/* The item does not need physics while waiting to be picked up */
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);

		/* The mesh should not be using collision for interaction *** Uses the Interaction Box */
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		/// * * Collision & Physics Props * * ///
		/* Clear all collision settings for the Collision Box */
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		/* Collsion Enabled should be turned off while the item/weapon is equipped */
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Falling:
		
		/// * * Collision & Physics Props * * ///
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

		/* Clear all collision settings for the Collision Box */
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		/* Collsion Enabled should be turned off while the item/weapon is equipped */
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AItem_Base::SetItemState(EItemState State)
{
	ItemState = State;
	
	/* Update the item properties when the state is changed  */
	SetItemProps(State);
}
