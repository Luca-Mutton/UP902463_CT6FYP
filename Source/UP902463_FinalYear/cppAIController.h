// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "cppAIController.generated.h"

/**
 * 
 */
UCLASS()
class UP902463_FINALYEAR_API AcppAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AcppAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns); //process any detected objects

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f; //sight perception radius size

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightMemorylength = 5.0f; //how long the AI rembers the detected object

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f; //how far the player has to be to lose sight.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldofView = 90.0f; //field of view, how far the Ai can see

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false; //has player been deteceted?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f; // distance to the player
};
