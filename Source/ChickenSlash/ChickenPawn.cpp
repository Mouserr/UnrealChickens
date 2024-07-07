// Fill out your copyright notice in the Description page of Project Settings.


#include "ChickenPawn.h"

// Sets default values
AChickenPawn::AChickenPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AChickenPawn::BeginPlay()
{
	Super::BeginPlay();

	PickDirection();
}

void AChickenPawn::TickMovement(float DeltaTime)
{
	FVector location = GetActorLocation();
	location += Direction * Speed * DeltaTime;
	SetActorLocation(location);
	NextDirectionChangeTime-=DeltaTime;
	
	if (NextDirectionChangeTime <= 0)
	{
		PickDirection();
	}
}

void AChickenPawn::PickDirection(FVector *ForcedDirection)
{
	Direction = ForcedDirection != nullptr ? *ForcedDirection : FVector(FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f), 0.0f);
	SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	NextDirectionChangeTime = FMath::FRandRange(-1.f, 1.f) + DirectionChangeInterval;
}

