#include "HP.h"


UHP::UHP()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Health = 100.0f;
}

float UHP::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser){
	Health -=DamageAmount;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Begin"));
	return Health;
}

void UHP::BeginPlay()
{
	Super::BeginPlay();
	
}


void UHP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

