// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class FALLANDCONTINUE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent*  Mesh;
	UFUNCTION(BlueprintCallable, Category = Shoot)
		void Shoot();
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ShootTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool NeedShoot;
	virtual void Tick(float DeltaTime) override;

};
