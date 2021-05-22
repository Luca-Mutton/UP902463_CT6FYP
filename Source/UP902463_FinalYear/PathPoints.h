// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PathPoints.generated.h"

UCLASS()
class UP902463_FINALYEAR_API APathPoints : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathPoints();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;


	//box component to visualize outline of path point
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;


	//point in path that enemy A.I. moves to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APathPoints* NextPathpoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);
};
