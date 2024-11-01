// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryEnums.h"
#include "InventoryComponent.generated.h"

class AWeapon_Base;
class AAmmoItem;
class AItem_Base;
class ANick_ShooterCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwapItemDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPE340_SHOOTER_NICK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Used for retrieving generic data that is need to access the anim instance */
	TObjectPtr<ANick_ShooterCharacter> OwningCharacter;
	
	/* Used to check to make sure there is ammo in the gun */
	bool HasAmmoInGun();

	/* Used to check that the player has ammo that is the same as the equipped weapon */
	bool HasAmmoType();

	void GetPickupItem(AItem_Base* PickupItem);

	void PickupAmmo(AAmmoItem* AmmoItem);

	void SwitchWeapons(AWeapon_Base* WeaponToBeSwitched);

	UFUNCTION(BlueprintCallable)
	void SwapInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);
	
	/* Map that keeps track of the ammo types held by the player using a key value pair */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory Properties | Ammo")
	TMap<EAmmoTypes, int32> AmmoMap;

	/* This distance outward from the camera to the desired interp location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory Properties | Item Props")
	float CameraInterpForaward;

	/* This distance up/down from the camera to the desired interp location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory Properties | Item Props")
	float CameraInterpUp;

	/* Unused for the moment */
	UPROPERTY(BlueprintAssignable, Category="Inventory Delegate")
	FSwapItemDelegate SwapItemDelegate;

protected:
	virtual void BeginPlay() override;

	/* Used to initialize the ammo map with default values on play */
	void InitialzeAmmoMap();

	/* Array of Item Base for the inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory Properties | Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<AItem_Base*> InventoryItems;

private:

	/* Starting Pistol Ammo Amount */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Inventory Properties | Ammo", meta = (AllowPrivateAccess = "true"))
	int32 DefaultPistolAmmo;

	/* Starting Rifle Ammo Amount */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Inventory Properties | Ammo", meta = (AllowPrivateAccess = "true"))
	int32 DefaultRifleAmmo;

	/* Starting Special Ammo Amount */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Inventory Properties | Ammo", meta = (AllowPrivateAccess = "true"))
	int32 DefaultSpecialAmmo;

	const int32 INVENTORY_CAPACITY{ 3 };

public:
	//FORCEINLINE TMap<EAmmoTypes, int32> GetAmmoMap() const { return AmmoMap; }
	FORCEINLINE TArray<AItem_Base*> GetInventory() { return InventoryItems; }
		
};
