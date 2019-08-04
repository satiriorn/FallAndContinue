#include "Thorn.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"

AThorn::AThorn(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;
	
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(28.0f);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>mesh(TEXT("/Game/StylizedDesertEnv/Meshes/s_plant_10.s_plant_10"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh ->SetStaticMesh(mesh.Object);
	RootComponent = SphereComponent;
	SphereComponent->SetSimulatePhysics(false);
	FVector Position;
	Position.X = 0.0f;
	Position.Y = 0.0f;
	Position.Z = 10.0f;
	SphereComponent->SetWorldLocation(Position);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThorn::OnOverlapBegin);
}

void AThorn::BeginPlay()
{
	Super::BeginPlay();
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules( EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	Mesh->AttachToComponent(RootComponent, TransformRules);
	
}

void AThorn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	 if((OtherActor!=nullptr)&&(OtherActor!=this)&&(OtherComp!=nullptr)){
		float BaseDamage = 10.0f;
		AController * EventInstigator= GetInstigatorController();
		TSubclassOf <class UDamageType> DamageTypeClass;
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, EventInstigator, OtherActor, DamageTypeClass);
	}
}


void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

