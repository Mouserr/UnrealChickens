// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrenadeProjectile.h"
#include "GameFramework/PlayerController.h"
#include "ChickenSlashPlayerController.generated.h"

UCLASS()
class AChickenSlashPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChickenSlashPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	
	/** Navigate player to the current mouse cursor location. */
	void RotateToMouseCursor();

	/** Input handlers for SetDestination action. */
	void Fire();
};


