// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeWeapon.h"
#include "Shield.h"
#include "HP.h"
#include "BunAssistant.generated.h"



UCLASS(config = Game)
class ABunAssistant : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera3D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom3D;
		
	virtual void Tick(float DeltaSeconds) override;
     
	
protected:
	void MoveRight(float Val);
	
	void MoveForward(float Value);
	
	void Sprint();
	
	void StopSprint();
	
	void SetSword();
	
	void ChangeSpace();

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	virtual void Landed(const FHitResult& Hit)override;
	
	//virtual void PostInitializeComponents() override;

	void UpdateAnimations();
	
	void GetShield();
	
	void DoubleJump(); 
	
	void GetSword();
	
	void SwichSpace();
	
	void MeleeAttack();
	
	void StopMeleeAttack();
	
	void GetState();
	
	void StartSlide();
	
	void StopSlide();
	
	void Yaw(float amount);
	
	void Pitch(float amount);
	
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	class UAnimSequence* RunWoundedAnimation;
		
	class UAnimSequence* IdleWoundedAnimation;
	
	class UAnimSequence* RunAnimation; 

	class UAnimSequence* IdleAnimation;
	
	class UAnimSequence* JumpAnimation;
	
	class UAnimSequence* DieAnimation;
	
	class UAnimSequence* WeaponSelection;
	
	class UAnimSequence* JumpAttack;
		
	class UAnimSequence* LowAttack;
	
	class UAnimSequence* NormalAttack;
	
	class UAnimSequence* SlideAnimation;
	
	class UAnimSequence* TopHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP;
	
	float Rotate;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
		UClass* ObjMeleeWeapon;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
		UClass* ObjShield;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
		AActor* CameraOne;
public:
	ABunAssistant();
	
	bool RunSpeed;
	
	UHP *Health;
	
	bool Run;
	
	bool StateMelee;
	
	bool RunTunnel;
	
	float TimeAnimationSlide;
	
	FTimerHandle InOutHandle;
	
	void BeginPlay();
	
	void CreateWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Varibl;
	
	float JumpCount;
	
	float JumpHeight;
	
	float TimeBetweenCameraChanges;
	
	float SmoothBlendTime;
	
	float TimeToNextCameraChange;
	
	bool SpaceState2D;
	
	bool fly;
	
	bool ChangeCamera;
	
	bool AttackBelow;
	
	bool EnableZoneWeapon;
	
	bool StateSlide;
	
	bool SpaceState3D;
	
	bool sprint;
	
	bool GetSwordAnimation;
	
	bool TopHitState;
	
	bool EnableZoneShield;
	
	float TimeGetSwords;
	
	float DeltaTime;
	
	float TimeAnimationAttack;
	
	bool AttackNormal;
	
	bool GetSwords;
	
	bool GetShields;
	
	bool Attack;
	
	bool Dead;
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
