// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item_Base.h"
#include "Character/InventoryComponent.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


AItem_Base::AItem_Base():
ItemName(FString("Default Weapon")),
ItemAmount(0),
ItemRarity(EItemRarity::EIR_Common),
ItemState(EItemState::EIS_PickupReady),
ItemInterpStartLocation(FVector(0.f)),
ItemTargetLocation(FVector(0.f)),
bInterpingItem(false),
ZCurveTime(.7f),
ItemXInterp(0.f),
ItemYInterp(0.f),
ItemInterpSpeed(15.f),
InitialYawOffset(0.f),
PickupType(EPickupType::EPT_Max)
{
	// True so we can interp the item location effect
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Widget"));
	PickupWidget->SetupAttachment(GetRootComponent());

}

void AItem_Base::StartItemCurve(ANick_ShooterCharacter* Character)
{
	// Stores the character reference
	ShooterCharacter = Character;

	// Starting location of the item
	ItemInterpStartLocation = GetActorLocation();

	bInterpingItem = true;

	// Interping Item State
	SetItemState(EItemState::EIS_Equipping);

	// Start the timer 
	GetWorldTimerManager().SetTimer(ItemInterpTimerHandle, this, &AItem_Base::FinishInterping, ZCurveTime);

	const float ItemTargetCompYaw{ static_cast<float>(ShooterCharacter->GetItemInterpTargetComp()->GetComponentRotation().Yaw) };
	const float ItemRotationYaw { static_cast<float>(GetActorRotation().Yaw) };

	// Initial Yaw offset between item and component on character.
	InitialYawOffset = ItemRotationYaw - ItemTargetCompYaw;
}

void AItem_Base::FinishInterping()
{
	bInterpingItem = false;
	
	if (ShooterCharacter)
	{
		ShooterCharacter->GetInventoryComp()->GetPickupItem(this);
	}
	
	// Zero out target location data
	ItemTargetLocation = FVector(0.f);
	// Set Scale back to default size
	SetActorScale3D(FVector(1.f));
}

void AItem_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	InterpItem(DeltaSeconds);
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
		
	case EItemState::EIS_Equipping:
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
	}
}

void AItem_Base::InterpItem(float DeltaTime)
{
	if (!bInterpingItem) return;

	if (ShooterCharacter && InterpZCurve)
	{
		// Time that has passed since the timer has started
		const float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimerHandle);
		// Get the curve value that corresponds to the current time
		const float CurveValue = InterpZCurve->GetFloatValue(ElapsedTime);

		// Get scalar Delta from start location to target location
		FVector ItemLocation = ItemInterpStartLocation;
		ItemTargetLocation = ShooterCharacter->GetTargetInterpLocation();
		// Delta Vector for Z Axis
		const FVector ItemToCamera{ FVector(0.f, 0.f, (ItemTargetLocation- ItemLocation).Z) };
		// Scalar to multiply with curve value
		const float DeltaZ = ItemToCamera.Size();

		/* Interpolates the X and Y Value */
		const FVector CurrentItemLocation{ GetActorLocation() };
		const float InterpXValue = FMath::FInterpTo(CurrentItemLocation.X, ItemTargetLocation.X, DeltaTime, ItemInterpSpeed);
		const float InterpYValue = FMath::FInterpTo(CurrentItemLocation.Y, ItemTargetLocation.Y, DeltaTime, ItemInterpSpeed);

		/* Set Item X and Y Values */
		ItemLocation.X = InterpXValue;
		ItemLocation.Y = InterpYValue;
		
		// Scaled Curve Value
		ItemLocation.Z += CurveValue * DeltaZ;
		
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		// Current component Rotation
		const FRotator ComponentRotation{ ShooterCharacter->GetItemInterpTargetComp()->GetComponentRotation() };
		FRotator ItemRotation{ 0.f, ComponentRotation.Yaw + InitialYawOffset, 0.f};
		SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

		if (ItemScaleCurve)
		{
			const float CurveScaleValue = ItemScaleCurve->GetFloatValue(ElapsedTime);

			/* Scale Item */
			SetActorScale3D(FVector(CurveScaleValue, CurveScaleValue, CurveScaleValue));
		}
	}
}

void AItem_Base::PlayPickupSound()
{
	if (PickupSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSFX, GetActorLocation());
	}
}

void AItem_Base::PlayPickupEffects()
{
	if (PickupVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, PickupVFX, Location);
	}
}

void AItem_Base::SetItemState(EItemState State)
{
	ItemState = State;
	
	/* Update the item properties when the state is changed  */
	SetItemProps(State);
}
