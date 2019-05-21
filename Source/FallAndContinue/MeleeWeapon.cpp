
// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer)
{
	AttackDamage = 1;
	Swinging = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("Mesh"));
	Mesh->SetStaticMesh(mesh.Object);
	
	RootComponent = Mesh;
	
	SpaceEnable= ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SpaceEnable"));
	SpaceEnable->InitSphereRadius(220.0f);
	SpaceEnable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	SpaceEnable->OnComponentBeginOverlap.AddDynamic(this,&AMeleeWeapon::OnOverlapBegin);
	SpaceEnable->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapEnd); 
}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		 	 ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			 Assistant->EnableZoneWeapon=true;
			 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
	 }

 }
 
 void AMeleeWeapon::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Assistant->EnableZoneWeapon=false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End"));
	}
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static ABunAssistant* Assistant; 
	Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	sss=Assistant->GetSwords;
	if(sss)
		Destroy();
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

