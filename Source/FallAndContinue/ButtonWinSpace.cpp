// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWinSpace.h"

// Sets default values
AButtonWinSpace::AButtonWinSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AButtonWinSpace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonWinSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

