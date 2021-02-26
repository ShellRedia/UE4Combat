// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4Combat01GameMode.h"
#include "UE4Combat01Character.h"
#include "UObject/ConstructorHelpers.h"

AUE4Combat01GameMode::AUE4Combat01GameMode()
{
	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
	
	DefaultPawnClass = AWarriorCharacter::StaticClass();
}
