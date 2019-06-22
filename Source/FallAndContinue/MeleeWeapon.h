// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BunAssistant.h"
#include "MeleeWeapon.generated.h"


UCLASS()
class FALLANDCONTINUE_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AMeleeWeapon(const FObjectInitializer& ObjectInitializer);

	TArray<AActor*> ThingsHit;

	bool State;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = SpaceEnable)
		USphereComponent* SpaceEnable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category =MeleeWeapon)
		UBoxComponent* BoxComponent;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category =MeleeWeapon)
		UStaticMeshComponent* Mesh;
		
		
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;
	
public:	

	FTimerHandle InOutHandle;
	bool GetSword;
	bool StMelee;
	void DestroyActor();

	
	virtual void Tick(float DeltaTime) override;
	

};
