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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =
		MeleeWeapon)
		float AttackDamage;

	TArray<AActor*> ThingsHit;

	bool Swinging;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = SpaceEnable)
		USphereComponent* SpaceEnable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category =MeleeWeapon)
		UBoxComponent* ProxBox;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category =MeleeWeapon)
		UStaticMeshComponent* Mesh;
		
		
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		/*
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void Prox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// You shouldn't need this unless you get a compiler error that it can't find this function.
	virtual int Prox_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	void Swing();
	void Rest();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool sss;
protected:

	virtual void BeginPlay() override;
	
public:	
	
	virtual void Tick(float DeltaTime) override;
	

};
