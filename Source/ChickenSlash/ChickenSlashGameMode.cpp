// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChickenSlashGameMode.h"
#include "ChickenSlashPlayerController.h"
#include "ChickenSlashCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChickenSlashGameMode::AChickenSlashGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AChickenSlashPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}