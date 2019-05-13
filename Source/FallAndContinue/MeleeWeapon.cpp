
// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer)
{
	AttackDamage = 1;
	Swinging = false;
	//WeaponHolder = NULL;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,
		TEXT("Mesh"));
	RootComponent = Mesh;

	ProxBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,
		TEXT("ProxBox"));
	ProxBox->OnComponentBeginOverlap.AddDynamic(this,
		&AMeleeWeapon::Prox);
	ProxBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

