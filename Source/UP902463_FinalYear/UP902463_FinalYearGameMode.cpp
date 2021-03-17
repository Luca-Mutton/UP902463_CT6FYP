// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UP902463_FinalYearGameMode.h"
#include "UP902463_FinalYearCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUP902463_FinalYearGameMode::AUP902463_FinalYearGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/MyUP902463_FinalYearCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
