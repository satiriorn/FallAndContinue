// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWinSpace.h"
#include "FallAndContinueGameMode.h"
#include "BunAssistant.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"

// Sets default values
AButtonWinSpace::AButtonWinSpace(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<ABunAssistant> BunAss(TEXT("/Game/Blueprints/Character/BunAssistantBP"));
	Bunassistant = BunAss.Class;
	SpaceEnable= ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SpaceEnable"));
	SpaceEnable->InitSphereRadius(140.0f);
	RootComponent = SpaceEnable;
	SpaceEnable->OnComponentBeginOverlap.AddDynamic(this,&AButtonWinSpace::OnOverlapBegin);; 
}

void AButtonWinSpace::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		AFallAndContinueGameMode* gm = (AFallAndContinueGameMode*)GetWorld()->GetAuthGameMode();
		gm->StateCharacter(true);
		ABunAssistant* a = GetWorld()->SpawnActor<ABunAssistant>(Bunassistant);
		a->RunSpeed = 3000.0f;
		//OurPlayerController->SetViewTargetWithBlend(Bunassistant, 4.0);
	 }
 }
 
void AButtonWinSpace::BeginPlay()
{
	Super::BeginPlay();
	
}

void AButtonWinSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

