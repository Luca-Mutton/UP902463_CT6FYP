// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UP902463_FinalYearCharacter.generated.h"

UCLASS(config=Game)
class AUP902463_FinalYearCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AUP902463_FinalYearCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

#pragma region Attacks and actions
	//allows character to punch
	void Punch();

	//determines if the character is currently zoomed in to their weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		bool hasPunched;


#pragma endregion

#pragma region Health: Functions and values
	//HP_System variables
	void StartDamage();

	//function for the damage that the player takes (can be called inside a blueprint)
	UFUNCTION(BlueprintCallable)
		void TakeDamage(float _damageAmount);

	void StartHeal();
	void Heal(float _healAmount);

	//kill player function (can be called inside a blueprint)
	UFUNCTION(BlueprintCallable)
		void Die();

	//Respawn player function (can be called inside a blueprint)
	UFUNCTION(BlueprintCallable)
		void Respawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		FVector respawnLocation;
#pragma endregion

#pragma region Abilities: functions and values
	//determines if a player has used an ability
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Abilities")
		bool usedAbility1;

	//the duration for the ability cooldown
	float abilityDuration;

	// CD = Cooldown(the cooldown time for this ability)
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Abilities")
		float abilityCDTime;

	//the timer handle for the character's first ability
	FTimerHandle ability1TimerHandle;

	//function to use ability
	void UseAbility1();

	//reset the character's ability to it's default state
	void ResetAbility1();

	//allow the character's ability to be used again
	void AbilityCDCompleted();

#pragma endregion 

#pragma region Player Stats
	//the amount of health Player has (can be edited in blueprints)
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Health")
		float PlayerHP;

	//character's current level
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Stats")
		int currentLevel;

	//the character's current experience Points
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Stats")
		float experiencePoints;

	//the total amount of EXP needed to level up
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Stats")
		float experienceToLevel;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void GainExperience(float _EXPAmount);

	//character's current Stength
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Stats")
		int StrengthStat;

	//character's current defense
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Stats")
		int DefenseStat;
#pragma endregion


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

