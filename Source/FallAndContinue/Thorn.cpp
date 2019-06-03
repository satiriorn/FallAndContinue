#include "Thorn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AThorn::AThorn(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/StylizedDesertEnv/Meshes/s_plant_10.s_plant_10"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh ->SetStaticMesh(mesh.Object);
	RootComponent = Mesh;
	SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
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
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage);
}


void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

