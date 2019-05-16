// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeWeapon.h"
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
	
	void SetSword();

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	virtual void Landed(const FHitResult& Hit)override;
	
	//virtual void PostInitializeComponents() override;

	void UpdateAnimations();
	
	void DoubleJump(); 
	
	void GetSword();
	
	void MeleeAttack();
	
	void StopMeleeAttack();
	
	void AnimationState();
	
	class UAnimSequence* RunWoundedAnimation;
		
	class UAnimSequence* IdleWoundedAnimation;
	
	class UAnimSequence* RunAnimation; 

	class UAnimSequence* IdleAnimation;
	
	class UAnimSequence* JumpAnimation;
	
	class UAnimSequence* DieAnimation;
	
	class UAnimSequence* WeaponSelection;
	
	class UAnimSequence* TopHit;
		
	class UAnimSequence* HitBelow;
	
	float HP;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
		UClass* ObjMeleeWeapon;
		
public:
	FTimerHandle InOutHandle;
	
	void BeginPlay();
	
	ABunAssistant();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Varibl;
	
	float JumpCount;
	
	float DeltaTime;
	
	float JumpHeight;
	
	bool fly;
	
	bool AttackBelow;
	
	bool EnableZoneWeapon;
	
	float TimeGetSwords;
	
	float TimeAnimationAttack;
	
	bool GetSwords;
	
	bool Attack;
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
