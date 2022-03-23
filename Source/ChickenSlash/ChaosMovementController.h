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

	UPROPERTY(EditAnywhere)
	int InfectedStartCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentInfectedCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Infect(AChickenPawn* chicken);
	void SpawnOnStart();
	AChickenPawn* SpawnChicken();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
private:
	UFUNCTION()
	void OnBeginOverlap(class AActor * SelfActor, class AActor * OtherActor);
	
	TArray<AChickenPawn *> Chickens;

};
