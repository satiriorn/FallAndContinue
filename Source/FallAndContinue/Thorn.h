// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "Thorn.generated.h"

UCLASS()
class FALLANDCONTINUE_API AThorn : public AActor
{
	GENERATED_BODY()
	
public:	
	AThorn(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION()
		void OnOverlap(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

public:	
	UStaticMeshComponent*  Mesh;
	
	USphereComponent* SphereComponent;
	
	virtual void Tick(float DeltaTime) override;

};
