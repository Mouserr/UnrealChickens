// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChickenPawn.h"
#include "GameFramework/Actor.h"
#include "ChaosMovementController.generated.h"

UCLASS()
class CHICKENSLASH_API AChaosMovementController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChaosMovementController();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AChickenPawn> ChickenBlueprint;

	UPROPERTY(EditAnywhere)
	float SpawnZ = 200.0f;

	UPROPERTY(EditAnywhere)
	float SpawnXMin;
	UPROPERTY(EditAnywhere)
	float SpawnXMax;
	UPROPERTY(EditAnywhere)
	float SpawnYMin;
	UPROPERTY(EditAnywhere)
	float SpawnYMax;

	UPROPERTY(EditAnywhere)
	int PreSpawnCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnOnStart();
	void SpawnChicken();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnChickenHit(class AActor * SelfActor, class AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	TArray<AChickenPawn *> Chickens;
	
};
