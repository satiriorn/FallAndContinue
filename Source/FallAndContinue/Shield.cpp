// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AShield::AShield(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Shield01SM.Shield01SM"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("Mesh"));
	Mesh->SetStaticMesh(mesh.Object);
	
	RootComponent = Mesh;
	
	SpaceEnable= ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SpaceEnable"));
	SpaceEnable->InitSphereRadius(220.0f);
	SpaceEnable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
	SpaceEnable->OnComponentBeginOverlap.AddDynamic(this,&AShield::OnOverlapBegin);
	SpaceEnable->OnComponentEndOverlap.AddDynamic(this, &AShield::OnOverlapEnd); 
}

void AShield::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		 	 ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			 Assistant->EnableZoneShield=true;
			 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Begin"));
	 }

 }
 
 void AShield::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		ABunAssistant* Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Assistant->EnableZoneShield=false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("End"));
	}
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	static ABunAssistant* Assistant; 
	static bool DestroyActor;
	Assistant = Cast<ABunAssistant>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	DestroyActor=Assistant->GetShields;
	if(DestroyActor)
		Destroy();
}

