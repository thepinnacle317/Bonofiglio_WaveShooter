// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Nick_ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Character/AttributeComponent.h"
#include "Character/InteractionComponent.h"
#include "Character/InventoryComponent.h"
#include "Character/ShooterCharacterComp.h"
#include "GameCore/Nick_ShooterPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ANick_ShooterCharacter::ANick_ShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and define the camera boom. (Pulls in toward character when colliding).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f; // Camera Follow Distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate the boom based on the controller.

	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	/* Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // The default end location of the spring arm by socket.
	FollowCamera->bUsePawnControlRotation = false; // Set camera to false, so it only follows the boom.

	/* Do not rotate the character mesh when the controller rotates */ 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	ItemInterpTargetComp = CreateDefaultSubobject<USceneComponent>(TEXT("Item Target Location"));
	ItemInterpTargetComp->SetupAttachment(GetFollowCamera());

	/* Rotate the character to player input movement */
	// TODO: This will change when strafe movement is implemented along with combat states.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .4f; // Control of the character while in the air.

	/* Core Gameplay Components */
	ShooterCharacterComp = CreateDefaultSubobject<UShooterCharacterComp>(TEXT("Shooter Character Component"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
}


void ANick_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnAiming.BindUObject(this, &ANick_ShooterCharacter::Aim);
	
	/* Set the Camera FOV to the value assigned in the shooter Comp */
	FollowCamera->FieldOfView = ShooterCharacterComp->DefaultCameraFOV;
	
	/* Set the character movement component to the value assigned in the shooter component */
	GetCharacterMovement()->MaxWalkSpeed = ShooterCharacterComp->DefaultCharacterSpeed;
}

void ANick_ShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Set the owning controller to be used for binding delegates.
	OwningController = Cast<ANick_ShooterPlayerController>(NewController);
}

void ANick_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Smoothly switch between the aiming and default FOV */
	InterpolateCameraFOV(DeltaTime);
	
	/* Update the sensitivity rate based on the state of aiming */
	ShooterCharacterComp->SetAimSensitivity();
}

FVector ANick_ShooterCharacter::GetTargetInterpLocation()
{
	return ItemInterpTargetComp->GetComponentLocation();
}

float ANick_ShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (GetAttributeComp())
	{
		if (GetAttributeComp()->GetHealth() - DamageAmount <= 0.f)
		{
			// Could be swapped for a clamp
			GetAttributeComp()->SetHealth(0.f);
			// TODO: Handle Death
		}
		else
		{
			GetAttributeComp()->SetHealth(GetAttributeComp()->GetHealth() - DamageAmount);
		}
	}
	
	return DamageAmount;
}

void ANick_ShooterCharacter::Aim()
{
	// Can decrease size of the reticle as well for more precise targeting.
}

void ANick_ShooterCharacter::InterpolateCameraFOV(float DeltaTime)
{
	if (ShooterCharacterComp->GetbIsAiming())
	{
		/* Aiming : Interpolate to the Aiming FOV value */
		ShooterCharacterComp->SetCurrentCameraFOV(FMath::FInterpTo(ShooterCharacterComp->GetCurrentCameraFOV(),
			ShooterCharacterComp->AimingCameraFOV, DeltaTime, ShooterCharacterComp->AimInterpSpeed));
	}
	else
	{
		/* Not Aiming : Interpolate to the Default FOV value */
		ShooterCharacterComp->SetCurrentCameraFOV(FMath::FInterpTo(ShooterCharacterComp->GetCurrentCameraFOV(),
			ShooterCharacterComp->DefaultCameraFOV, DeltaTime, ShooterCharacterComp->AimInterpSpeed));
	}
	/* Set the Cameras FOV Value */
	GetFollowCamera()->SetFieldOfView(ShooterCharacterComp->GetCurrentCameraFOV());
}

