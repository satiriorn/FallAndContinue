// Fill out your copyright notice in the Description page of Project Settings.


#include "StickWhisk.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"
#include "BunAssistant.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStickWhisk::AStickWhisk(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/Platformer/Meshes/Beanstalks/Pl_StoneBlock_03.Pl_StoneBlock_03"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("Mesh"));
	Mesh->SetStaticMesh(mesh.Object);
	RootComponent = Mesh;

	Position.X = 0.0f;
	Position.Y = 0.0f;
	Position.Z = 0.0f;
	
	Rotation.Yaw = 0.0f;
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 90.0f;
	speed = 1.0f;

	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetWorldLocationAndRotation(Position, Rotation);
}

// Called every frame
void AStickWhisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(rotate)
		Rotation.Yaw+= speed;
	else 
		Rotation.Yaw-=speed;
	SetActorRotation(Rotation); 
}

