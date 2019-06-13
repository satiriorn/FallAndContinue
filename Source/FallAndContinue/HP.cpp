#include "HP.h"
#include "DrawDebugHelpers.h"

UHP::UHP()
{
	PrimaryComponentTick.bCanEverTick = true;
	AActor* MyOwner = GetOwner();
	if(MyOwner){
		MyOwner->OnTakeAnyDamage.AddDynamic(this,&UHP::TakeDamage);
	}
	Health = 100.0f;
}

void UHP::TakeDamage(AActor* DamagedActor, float Damage,const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser){
	Health -=Damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Damage"));
}

void UHP::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Begin"));
}


void UHP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Tick"));
}

