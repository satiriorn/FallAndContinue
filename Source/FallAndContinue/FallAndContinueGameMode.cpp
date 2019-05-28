// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FallAndContinueGameMode.h"
#include "FallAndContinueCharacter.h"
#include "BunAssistant.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AFallAndContinueGameMode::AFallAndContinueGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
	static ConstructorHelpers::FClassFinder<ABunAssistant> BunAss(TEXT("/Game/Blueprints/Character/BunAssistantBP"));
	static ConstructorHelpers::FClassFinder<AFallAndContinueCharacter>bun(TEXT("/Game/Blueprints/Character/Bun"));
	Bunassistant = BunAss.Class;
	Bun = bun.Class;
	DefaultPawnClass = Bun;	
}
void AFallAndContinueGameMode::StateCharacter(bool State){
	if(State==true){
		DefaultPawnClass = Bunassistant;
		//ABunAssistant a = GetWorld()->SpawnActor(Bunassistant, FVector(), FRotator());
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(Bunassistant, 0);
		//OurPlayerController->UnPossess();
		//OurPlayerController->Possess(a);
		//OurPlayerController->SetViewTargetWithBlend(Bunassistant, 4.0);
	}	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Begin"));
}