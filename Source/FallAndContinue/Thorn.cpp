#include "Thorn.h"
#include "Kismet/GameplayStatics.h"

AThorn::AThorn()
{
	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("FriendlyName"));
	SphereComponent->InitSphereRadius(25.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->BeginComponentOverlap.AddDynamic(this, &AThorn::OnOverlapBegin);
}

void AThorn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThorn::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float BaseDamage = 10.0f;
	AController * EventInstigator;
	AActor * DamageCauser;
	TSubclassOf < class UDamageType > DamageTypeClass;
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, EventInstigator, DamageCauser, DamageTypeClass);
}


void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

