// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InteractionComponent.h"
#include "Actors/Item_Base.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


UInteractionComponent::UInteractionComponent() :
/* Member Initializer List : Put these in order by initialization */
	TraceRate(.05f),
	InteractionTraceDistance(500.f),
	WidgetDisappearDistance(400.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	/* Start the interaction timer *** More performant method than using tick */
	GetWorld()->GetTimerManager().SetTimer(InteractionHandle, this, &UInteractionComponent::StartInteractionTimer, TraceRate, true);
	
}

bool UInteractionComponent::InteractionTrace(FHitResult& OutHitResult)
{
	// Get the current size of the viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get the center of the screen.
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	
	CrosshairLocation.Y -= 50.f; // Move the crosshair up 50 units on the screen.

	/* These get set when we call deproject and will be used to get world vectors */
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Transforms the given 2D screen space coordinate into a 3D world-space point and direction.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetOwner(), 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		/* Line Trace from Crosshair world location */
		const FVector Start{ CrosshairWorldPosition	};
		const FVector End{ Start + CrosshairWorldDirection * InteractionTraceDistance };
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			return true;
		}
	}
	return false;
}

void UInteractionComponent::StartInteractionTimer()
{
	InteractionTrace(ItemResults);
	if (ItemResults.bBlockingHit)
	{
		HitItem = Cast<AItem_Base>(ItemResults.GetActor());
		if (HitItem && HitItem->GetPickupWidget())
		{
			// Set Widget visibilty
			HitItem->GetPickupWidget()->SetVisibility(true);
			
		}
		/* AItem Was Hit last frame */
		if (HitItemLastFrame)
		{
			if (HitItem != HitItemLastFrame)
			{
				/* Hitting Different AItem_Base this frame than last or it is null */
				HitItemLastFrame->GetPickupWidget()->SetVisibility(false);
            }
		}
		/* Store a reference to the last hit item */
		HitItemLastFrame = HitItem;
	}
	else
	{
		/* Distance based failsafe for removing a item pickup Widget */
		float DistanceToItem = GetOwner()->GetDistanceTo(HitItem);
		if(DistanceToItem > WidgetDisappearDistance)
		{
			HitItem->GetPickupWidget()->SetVisibility(false);
		}
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

