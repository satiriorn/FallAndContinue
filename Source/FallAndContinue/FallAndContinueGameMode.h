// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FallAndContinueGameMode.generated.h"

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of FallAndContinueCharacter
 */
UCLASS(minimalapi)
class AFallAndContinueGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFallAndContinueGameMode(const FObjectInitializer& ObjectInitializer);
	UPROPERTY()
	UClass* Bunassistant;
	UPROPERTY()
	UClass* Bun;
	void StateCharacter(bool State);
};
