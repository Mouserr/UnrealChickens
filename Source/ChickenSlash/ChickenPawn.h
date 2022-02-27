// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChickenPawn.generated.h"

UCLASS()
class CHICKENSLASH_API AChickenPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChickenPawn();

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float DirectionChangeInterval = 2;
	
	UPROPERTY(EditAnywhere)
	int InfectionChance = 2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Infected;

	void TickMovement(float DeltaTime);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector Direction;
	float NextDirectionChangeTime;
	void PickDirection(FVector *ForcedDirection = nullptr);

};
