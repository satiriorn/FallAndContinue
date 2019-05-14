
// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer)
{
	AttackDamage = 1;
	Swinging = false;
	//WeaponHolder = NULL;
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("Mesh"));
	Mesh->SetStaticMesh(mesh.Object);
	RootComponent = Mesh;

	ProxBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,
		TEXT("ProxBox"));

	SpaceEnable= ObjectInitializer.CreateDefaultSubobject<USphereComponent>
		(this, TEXT("SpaceEnable"));
	SpaceEnable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	ProxBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
	ProxBox->OnComponentBeginOverlap.AddDynamic(this,&AMeleeWeapon::OnOverlapBegin);


}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {

	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
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

