// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Engine/World.h"


AGun::AGun()
{

	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ShootTime = 0.0f;
	NeedShoot = false;
	Reload = 2.0f;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::Shoot()
{
	if (ShootTime <= 0.0f) {
		NeedShoot = true;
		Location = GetActorLocation();
		FRotator rotation = GetActorRotation();
		UWorld *WRLD = GetWorld();
		WRLD->SpawnActor(ActorToSpawn, &Location, &rotation);
		ShootTime = Reload;
	}
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ShootTime > 0.0f)
		ShootTime -= DeltaTime;
	else
		Shoot();
}

