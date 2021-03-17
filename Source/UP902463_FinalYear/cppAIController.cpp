// Fill out your copyright notice in the Description page of Project Settings.


#include "cppAIController.h"
#include "EnemyAI.h"
#include "PathPoints.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UP902463_FinalYearCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AcppAIController::AcppAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldofView;
	SightConfig->SetMaxAge(AISightMemorylength);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AcppAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AcppAIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error Occured"));
	}

}

void AcppAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}

void AcppAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AEnemyAI* Enemy = Cast<AEnemyAI>(GetPawn());

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	}

	//tells AI to move to pathpoint
	if (Enemy->NextPathPoint != nullptr && bIsPlayerDetected == false)
	{
		MoveToActor(Enemy->NextPathPoint, 5.0f); //how close the AI moves to target point before it stops
	}

	//if player is seen, chase player
	else if (bIsPlayerDetected == true)
	{
		AUP902463_FinalYearCharacter* Player = Cast<AUP902463_FinalYearCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 5.0f);
	}
}

FRotator AcppAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

//event handler for player detection
void AcppAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}

	bIsPlayerDetected = true;
}
