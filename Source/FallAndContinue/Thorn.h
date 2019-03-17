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
	// Sets default values for this actor's properties
	AThorn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent*  Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USphereComponent* SphereComponent;
	virtual void Tick(float DeltaTime) override;

};
