// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FallAndContinueGameMode.h"
#include "FallAndContinueCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AFallAndContinueGameMode::AFallAndContinueGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
	static ConstructorHelpers::FClassFinder<AFallAndContinueCharacter>bun(TEXT("/Game/Blueprints/Character/Bun"));
	DefaultPawnClass = bun.Class;	
}