// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChickenSlashPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "ChickenSlashCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AChickenSlashPlayerController::AChickenSlashPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AChickenSlashPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	RotateToMouseCursor();
}

void AChickenSlashPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AChickenSlashPlayerController::Fire);

	InputComponent->BindAxis("MoveForward",this,  &AChickenSlashPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AChickenSlashPlayerController::MoveRight);
	InputComponent->BindAction("Restart", IE_Pressed,this,  &AChickenSlashPlayerController::RestartGame);
}

void AChickenSlashPlayerController::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
}


void AChickenSlashPlayerController::MoveForward(float Axis)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	GetPawn()->AddMovementInput(Direction, Axis);
}

void AChickenSlashPlayerController::MoveRight(float Axis)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	GetPawn()->AddMovementInput(Direction, Axis);
}


void AChickenSlashPlayerController::RotateToMouseCursor()
{
	if (AChickenSlashCharacter* MyPawn = Cast<AChickenSlashCharacter>(GetPawn()))
	{
		if (MyPawn->GetCursorToWorld())
		{
			FVector Goal = MyPawn->GetCursorToWorld()->GetComponentLocation();
			FVector ActorLocation = MyPawn->GetActorLocation();
			FVector Direction = Goal - ActorLocation;
			Direction.Z = 0;
			Direction.Normalize();
			MyPawn->SetActorRotation(Direction.Rotation());
		}
	}
}


void AChickenSlashPlayerController::Fire()
{
	if (AChickenSlashCharacter* MyPawn = Cast<AChickenSlashCharacter>(GetPawn()))
	{
		MyPawn->LaunchGrenade();
	}
}