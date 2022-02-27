// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosMovementController.h"

// Sets default values
AChaosMovementController::AChaosMovementController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChaosMovementController::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AChaosMovementController::SpawnChicken,
		1,true);

	SpawnOnStart();
}

void AChaosMovementController::SpawnChicken()
{
	float RandX = FMath::RandRange(SpawnXMin, SpawnXMax);
	float RandY = FMath::RandRange(SpawnYMin, SpawnYMax);

	FVector SpawnPosition = FVector(RandX, RandY, SpawnZ);
	FActorSpawnParameters SpawnInfo; 
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Forces the pawn to spawn even if colliding
	AChickenPawn *chicken = GetWorld()->SpawnActor<AChickenPawn>(ChickenBlueprint.Get(), SpawnPosition, FRotator(), SpawnInfo);
	chicken->OnActorHit.AddDynamic(this, &AChaosMovementController::OnChickenHit);
	Chickens.Add(chicken);	
}

void AChaosMovementController::OnChickenHit(class AActor * SelfActor, class AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	AChickenPawn *Self = Cast<AChickenPawn>(SelfActor);

	if (Self != nullptr)
	{
		if (OtherActor->IsA(AChickenPawn::StaticClass()))
		{
			AChickenPawn* Other = Cast<AChickenPawn>(OtherActor);
			if (Other->Infected)
			{
				Self->Infected = true;
			}
			else if (Self->Infected)
			{
				Other->Infected = true;
			}
		}
	}	
}

void AChaosMovementController::SpawnOnStart()
{
	for (int i = 0; i < PreSpawnCount; i++)
	{
		SpawnChicken();
	}
}

// Called every frame
void AChaosMovementController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto chicken : Chickens)
	{
		if (chicken != nullptr)
		{
			chicken->TickMovement(DeltaTime);
		}
	}
}
