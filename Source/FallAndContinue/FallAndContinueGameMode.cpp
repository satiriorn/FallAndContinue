// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FallAndContinueGameMode.h"
#include "FallAndContinueCharacter.h"

AFallAndContinueGameMode::AFallAndContinueGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AFallAndContinueCharacter::StaticClass();	
}
