// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechRecognition.h"#include "Bun.h"

// Sets default values
ABun::ABun()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

