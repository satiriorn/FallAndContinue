#include "Thorn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AThorn::AThorn()
{
	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("FriendlyName"));
	SphereComponent->InitSphereRadius(25.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThorn::OnOverlap);
}

void AThorn::BeginPlay()
{
	Super::BeginPlay();

}

void AThorn::OnOverlap(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float BaseDamage = 10.0f;
	AController * EventInstigator= GetInstigatorController();
	TSubclassOf < class UDamageType > DamageTypeClass;
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, EventInstigator, OtherActor, DamageTypeClass);
}


void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

