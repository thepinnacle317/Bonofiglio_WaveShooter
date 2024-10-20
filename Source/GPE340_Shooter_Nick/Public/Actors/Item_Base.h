// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupType.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"

class ANick_ShooterCharacter;
/* Forward Declarations */
class UWidgetComponent;
class UBoxComponent;

/* Item Rarity Enum *** Used for defining how powerful, and likely it is to find this item */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Epic UMETA(DisplayName = "Epic"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_Max UMETA(DisplayName = "Default Max")
};

/* Item State Enum *** Used for defining the state of an item for changing physics and collision as well as VFX */
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_PickupReady UMETA(DisplayName = "Pickup Ready"),
	EIS_Equipping UMETA(DisplayName = "Equipping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_Max UMETA(DisplayName = "Default Max")
};

UCLASS()
class GPE340_SHOOTER_NICK_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:
	AItem_Base();

	/* Called from the character class */
	void StartItemCurve(ANick_ShooterCharacter* Character);

	/* Called when Item Interp Timer has finished */
	void FinishInterping();

	void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	/* Used to visually set the value of the item rarity in the pickup widget */
	void SetItemRarity();

	/* Sets the properties of the item based on its state *** ie. physics and collision */
	void SetItemProps(EItemState State);

	/* Handles the interpolation of the item when in the Equipping State */
	void InterpItem(float DeltaTime);

private:
	/* Used for setting the Weapon Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ItemMesh;
	
	/* Collision Box used to line trace against */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CollisionBox;

	/* World Widget that displays when the player looks at the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PickupWidget;

	/* Variable Used to assign the name of the item and show in the pickup widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	/* Represents the amount of ammo on a weapon or amount dropped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount;

	/* Item Rarity is used to show a visual representation throughout the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	/* Used for determining the visual representation in the pickup widget, this is set by the Rarity Enum */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveRarity;

	/* State that the item is currently in */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	/* Curve Table used for the Items Z Location when Interpolating */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> InterpZCurve;

	/* The location where the item starts interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	/* The location for the desired target in front of the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	FVector ItemTargetLocation;

	/* True when Interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	bool bInterpingItem;

	/* Plays when we start interping */
	FTimerHandle ItemInterpTimerHandle;

	/* Duration of the curve and timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	/* Character Reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties | Do Not Edit", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ANick_ShooterCharacter> ShooterCharacter;

	/* Used for adjusting the Items X and Y when Interpolating */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	float ItemXInterp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	float ItemYInterp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	float ItemInterpSpeed;

	/* Initial Yaw offset between item target comp and item */
	float InitialYawOffset;

	/* Curve that is used to scale the item when interpolating */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties | Interpolation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> ItemScaleCurve;

	/* Enum that classifies the type of item that the pickup is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties | Pickup Type", meta = (AllowPrivateAccess = "true"))
	EPickupType PickupType;

	

public:
	FORCEINLINE TObjectPtr<UWidgetComponent> GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE TObjectPtr<UBoxComponent> GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	void SetItemState(EItemState State);
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetItemMesh() const { return ItemMesh; }
	
};
