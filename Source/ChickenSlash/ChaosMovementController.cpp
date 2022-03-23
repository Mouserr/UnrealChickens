// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosMovementController.h"

#include "Kismet/GameplayStatics.h"

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
	/*FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AChaosMovementController::SpawnChicken,
		1,true);
		*/

	SpawnOnStart();
}

void AChaosMovementController::Infect(AChickenPawn* chicken)
{
	if (!chicken->Infected)
	{
		chicken->Infected = true;
		CurrentInfectedCount++;
	}
}

AChickenPawn* AChaosMovementController::SpawnChicken()
{
	float RandX = FMath::RandRange(SpawnXMin, SpawnXMax);
	float RandY = FMath::RandRange(SpawnYMin, SpawnYMax);

	FVector SpawnPosition = FVector(RandX, RandY, SpawnZ);
	FActorSpawnParameters SpawnInfo; 
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // Forces the pawn to spawn even if colliding
	AChickenPawn *chicken = GetWorld()->SpawnActor<AChickenPawn>(ChickenBlueprint.Get(), SpawnPosition, FRotator(), SpawnInfo);

	chicken->OnActorBeginOverlap.AddDynamic(this, &AChaosMovementController::OnBeginOverlap);
	
	Chickens.Add(chicken);
	return chicken;
}

void AChaosMovementController::OnBeginOverlap(class AActor * SelfActor, class AActor * OtherActor)
{
	if (AChickenPawn *Self = Cast<AChickenPawn>(SelfActor))
	{
		if (AChickenPawn* Other = Cast<AChickenPawn>(OtherActor))
		{
			if (Other->Infected)
			{
				Infect(Self);
			}
			else if (Self->Infected)
			{
				Infect(Other);
			}
		}
	}	
}

void AChaosMovementController::SpawnOnStart()
{
	for (int i = 0; i < PreSpawnCount; i++)
	{
		AChickenPawn* chicken = SpawnChicken();
		if (CurrentInfectedCount < InfectedStartCount)
		{
			Infect(chicken);
		}
	}
}

// Called every frame
void AChaosMovementController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AChickenPawn *> toRemove;
	for (auto chicken : Chickens)
	{
		if (chicken != nullptr)
		{
			if (!IsValid(chicken))
			{
				toRemove.Add(chicken);
			}
			else
			{
				chicken->TickMovement(DeltaTime);
			}
		}
	}

	for (auto chicken : toRemove)
	{
		if (chicken->Infected)
		{
			CurrentInfectedCount--;
		}
		
		Chickens.Remove(chicken);
	}

	if (Chickens.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
	}
}

