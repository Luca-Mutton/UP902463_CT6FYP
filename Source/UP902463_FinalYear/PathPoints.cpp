// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPoints.h"
#include "EnemyAI.h"

// Sets default values
APathPoints::APathPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APathPoints::OnPlayerEnter);
}

// Called when the game starts or when spawned
void APathPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//the path patrol for Ai and how it moves to different locations.
void APathPoints::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	AEnemyAI* Enemy = nullptr;

	if (OtherActor != nullptr)
	{
		Enemy = Cast<AEnemyAI>(OtherActor);
		if (Enemy != nullptr)
		{
			Enemy->NextPathPoint = NextPathpoint;
		}
	}
}

