// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "FallAndContinueCharacter.generated.h"

class UTextRenderComponent;
/**
 * This class is the default character for FallAndContinue, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AFallAndContinueCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;

	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* FallAnimation;

	void UpdateAnimation();

	void MoveRight(float Value);

	void UpdateCharacter();

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
 
	virtual void Landed(const FHitResult& Hit)override;

public:
	AFallAndContinueCharacter();
	UFUNCTION(BlueprintCallable, Category= Jump)
		void DoubleJump();
	UFUNCTION()
		void OffShoot();
	UFUNCTION()
		void TimeGod();
	UFUNCTION(BlueprintCallable, Category= Shoot)
		void Shoot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int JumpCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GeneralCountJump;
	UPROPERTY()
		bool NeedShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GodTrue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool moveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float God;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ShootTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimerGod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeDeadAnim;

	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
