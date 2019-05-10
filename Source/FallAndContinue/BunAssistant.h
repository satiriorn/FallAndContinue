// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BunAssistant.generated.h"


UCLASS(config = Game)
class ABunAssistant : public ACharacter
{
	GENERATED_BODY()


		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
		
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	void MoveRight(float Val);

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void UpdateAnimations();
	
	class UAnimSequence* RunAnimation; 

	class UAnimSequence* IdleAnimation;
	
public:
	void BeginPlay();
	
	ABunAssistant();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Varibl;
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
