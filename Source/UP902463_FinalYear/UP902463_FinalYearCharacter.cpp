// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UP902463_FinalYearCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// AUP902463_FinalYearCharacter

AUP902463_FinalYearCharacter::AUP902463_FinalYearCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


		//the player's max HP
	PlayerHP = 1.00f;

	//abilities
	usedAbility1 = false;
	abilityDuration = 1.0f;
	abilityCDTime = 5.0f;

	//attack
	hasPunched = false;

	//player stats
	currentLevel = 1;
	experiencePoints = 0.0f;
	experienceToLevel = 1000.0f;

	StrengthStat = 1;
	DefenseStat = 1;

}

//////////////////////////////////////////////////////////////////////////
// Input

#pragma region Ability 1: Functions
//heal the player
void AUP902463_FinalYearCharacter::UseAbility1()
{
	if (!usedAbility1)
	{
		usedAbility1 = true;
		Heal(0.4f);

		GetWorld()->GetTimerManager().SetTimer(ability1TimerHandle, this, &AUP902463_FinalYearCharacter::ResetAbility1, abilityDuration, false);
	}
}

//Timer for cooldown of ability
void AUP902463_FinalYearCharacter::ResetAbility1()
{
	GetWorld()->GetTimerManager().SetTimer(ability1TimerHandle, this, &AUP902463_FinalYearCharacter::AbilityCDCompleted, abilityCDTime, false);
}

void AUP902463_FinalYearCharacter::AbilityCDCompleted()
{
	usedAbility1 = false;
}
#pragma endregion

void AUP902463_FinalYearCharacter::GainExperience(float _EXPAmount)
{
	experiencePoints += _EXPAmount;

	//checks if experience is higher than the experience to level up
	if (experiencePoints >= experienceToLevel)
	{
		++currentLevel;
		experiencePoints -= experienceToLevel;
		experienceToLevel += 500.f;
	}
}

void AUP902463_FinalYearCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Heal and damage test
	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AUP902463_FinalYearCharacter::StartHeal);
	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &AUP902463_FinalYearCharacter::StartDamage);

	//use abilites
	PlayerInputComponent->BindAction("ActivateAbility1", IE_Pressed, this, &AUP902463_FinalYearCharacter::UseAbility1);

	//attacking
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AUP902463_FinalYearCharacter::Punch);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUP902463_FinalYearCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUP902463_FinalYearCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUP902463_FinalYearCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUP902463_FinalYearCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUP902463_FinalYearCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUP902463_FinalYearCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUP902463_FinalYearCharacter::OnResetVR);

	
}


void AUP902463_FinalYearCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUP902463_FinalYearCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUP902463_FinalYearCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

#pragma region Player take damage and heal
void AUP902463_FinalYearCharacter::Punch()
{
	hasPunched = true;
}


void AUP902463_FinalYearCharacter::StartDamage()
{
	TakeDamage(0.02f);
}

void AUP902463_FinalYearCharacter::TakeDamage(float _damageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points"), _damageAmount);
	PlayerHP -= _damageAmount;
	if (PlayerHP < 0.00f)
	{
		PlayerHP = 0.00f;
		Die();
	}
}

void AUP902463_FinalYearCharacter::StartHeal()
{
	Heal(0.02f);
}

void AUP902463_FinalYearCharacter::Heal(float _healAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("We are healing for %f points"), _healAmount);
	PlayerHP += _healAmount;
	if (PlayerHP > 1.00f)
	{
		PlayerHP = 1.00f;
	}
}

void AUP902463_FinalYearCharacter::Die()
{
	Respawn();
}

void AUP902463_FinalYearCharacter::Respawn()
{
	PlayerHP = 1.0f;
	SetActorLocation(respawnLocation);
}
#pragma endregion

void AUP902463_FinalYearCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUP902463_FinalYearCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUP902463_FinalYearCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUP902463_FinalYearCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
