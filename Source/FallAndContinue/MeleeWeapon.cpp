
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
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("Mesh"));
	Mesh->SetStaticMesh(mesh.Object);
	
	RootComponent = Mesh;
	
	SpaceEnable= ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SpaceEnable"));
	SpaceEnable->InitSphereRadius(140.0f);
	SpaceEnable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	SpaceEnable->OnComponentBeginOverlap.AddDynamic(this,&AMeleeWeapon::OnOverlapBegin);
	SpaceEnable->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapEnd); 
}

void AMeleeWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		 	 ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			 Assistant->EnableZoneWeapon=true;
	 }

 }
 
 void AMeleeWeapon::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Assistant->EnableZoneWeapon=false;
	}
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static ABunAssistant* Assistant; 
	static bool DestroyActor;
	Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	DestroyActor=Assistant->GetSwords;
	if(DestroyActor)
		Destroy();
}


