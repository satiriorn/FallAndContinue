
// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer)
{
	State = false;
	
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
	Mesh->OnComponentHit.AddDynamic(this,&AMeleeWeapon::OnHit);
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

void AMeleeWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	 ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComponent!=nullptr)&&(OtherActor!=Assistant)){
		float BaseDamage = 10.0f;
		AController * EventInstigator= GetInstigatorController();
		TSubclassOf <class UDamageType> DamageTypeClass;
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, EventInstigator, OtherActor, DamageTypeClass);
	}
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	GetSword=false;
	StMelee=false;
}

void AMeleeWeapon::DestroyActor(){
	Destroy();
}


void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABunAssistant* Assistant= Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Assistant){
		State = Assistant->StateMelee;
	}
	if(State&&StMelee==false)
	{
		SpaceEnable->DestroyComponent();
		GetWorldTimerManager().SetTimer(InOutHandle, this, &AMeleeWeapon::DestroyActor, 1.1, false);
		StMelee = true;
	}
}