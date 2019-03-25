// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceBun.generated.h"

/**
 * 
 */
UCLASS()
class FALLANDCONTINUE_API UGameInstanceBun : public UGameInstance
{
	GENERATED_BODY()
public:
	UGameInstanceBun();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CountJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpPrec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HpPrec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CountRestart;
	UFUNCTION()
		void HpRate();
	UFUNCTION()
		void JumpRate();

};
