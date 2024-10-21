// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

class AItem_Base;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GPE340_SHOOTER_NICK_API IPickupInterface
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void SetOverlappingItem(AItem_Base* PickupItem);
};
