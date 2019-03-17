
#include "Thorn.h"


AThorn::AThorn()
{
	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("FriendlyName"));
	SphereComponent->InitSphereRadius(25.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
}

void AThorn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

