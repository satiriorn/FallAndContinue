// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StickWhisk.generated.h"

UCLASS()
class FALLANDCONTINUE_API AStickWhisk : public AActor
{
	GENERATED_BODY()
	

		
public:	
	// Sets default values for this actor's properties
	AStickWhisk(const FObjectInitializer& ObjectInitializer);
protected:

	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Rotation)
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Rotation)
	bool rotate;
	FVector Position;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
