
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
	//ProxBox->OnComponentBeginOverlap.AddDynamic(this,&AMeleeWeapon::Prox);
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
/*
int AMeleeWeapon::Prox_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// don't hit non root components 
	if (OtherComp != OtherActor->GetRootComponent())
	{
		return -1;
	}

	// avoid hitting things while sword isn't swinging, 
	// avoid hitting yourself, and 
	// avoid hitting the same OtherActor twice 
	if (Swinging && OtherActor != (AActor *)WeaponHolder &&
		!ThingsHit.Contains(OtherActor))
	{
		OtherActor->TakeDamage(AttackDamage + WeaponHolder->BaseAttackDamage, FDamageEvent(), NULL, this);
		ThingsHit.Add(OtherActor);
	}

	return 0;
}*/

void AMeleeWeapon::Swing()
{
	ThingsHit.Empty();  // empty the list 
	Swinging = true;
}

void AMeleeWeapon::Rest()
{
	ThingsHit.Empty();
	Swinging = false;
}

