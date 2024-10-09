// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/* Forward Declarations */
class AItem_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType )
class GPE340_SHOOTER_NICK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FTimerHandle InteractionHandle;

	UPROPERTY()
	FHitResult ItemResults;

	/* The previous actor that was traced */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction | Hit Actors")
	TObjectPtr<AItem_Base> HitItemLastFrame;

	/* The current Item that is hit by the interaction trace *** Could be null */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction | Hit Actors")
	TObjectPtr<AItem_Base> HitItem;

	/* Variable used for by designer to decide on how far interaction should occur */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float InteractionTraceDistance;

	/* Line Trace for items under the crosshairs */
	UFUNCTION()
	bool InteractionTrace(FHitResult& OutHitResult);
	void StartInteractionTimer();

	/* The frequency that the trace timer will run *** Lower number means it consumes more performance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float TraceRate;

	/* How far the player must be from the Item before it will dissappear based on distance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trace Properties")
	float WidgetDisappearDistance;

protected:
	virtual void BeginPlay() override;
	
	

	
		
};
