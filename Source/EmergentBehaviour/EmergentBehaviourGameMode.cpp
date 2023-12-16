// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmergentBehaviourGameMode.h"
#include "EmergentBehaviourPlayerController.h"
#include "EmergentBehaviourCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEmergentBehaviourGameMode::AEmergentBehaviourGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEmergentBehaviourPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}