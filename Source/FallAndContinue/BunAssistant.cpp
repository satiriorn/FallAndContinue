// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BunAssistant.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

using namespace std;

class USkeletalMeshSocket;

ABunAssistant::ABunAssistant()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	
	JumpCount = 0;
	JumpHeight = 1100.f;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SKmodel(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/CommonerSK.CommonerSK"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>RunAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Sprint_MagicWandAnim.Sprint_MagicWandAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>RunwoundedAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Run_MagicWandAnim.Run_MagicWandAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>IdleAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Idle_MagicWandAnim.Idle_MagicWandAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>JumpAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/JumpAir_noWeaponAnim.JumpAir_noWeaponAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>IdlewoundedAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Dizzy_MagicWandAnim.Dizzy_MagicWandAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>DieAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/Die_noWeaponAnim.Die_noWeaponAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>SelectionWeaponAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/PickUp_noWeaponAnim.PickUp_noWeaponAnim"));
	
	JumpAnimation =JumpAnim.Object;
	RunAnimation = RunAnim.Object;
	RunWoundedAnimation = RunwoundedAnim.Object;
	IdleWoundedAnimation = IdlewoundedAnim.Object;
	IdleAnimation = IdleAnim.Object;
	DieAnimation = DieAnim.Object;
	WeaponSelection = SelectionWeaponAnim.Object;
	
	HP=112.0f;
	GetMesh()->SetSkeletalMesh(SKmodel.Object);

	EnableZoneWeapon = false;
}

void ABunAssistant::GetSword()
{
	FName fnWeaponSocket = TEXT("RightWeaponShield");
	if(EnableZoneWeapon)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(ObjMeleeWeapon, FVector(), FRotator());
		if(MeleeWeapon){
			MeleeWeapon->Mesh->SetSimulatePhysics(false);
			MeleeWeapon->Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			MeleeWeapon->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), fnWeaponSocket);
			}
	}
}
void ABunAssistant::BeginPlay()
{
	Super::BeginPlay();
}

void ABunAssistant::UpdateAnimations()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	static UAnimSequence* DesiredAnimation;
	
	if(PlayerSpeedSqr>0.0f&&DesiredAnimation!=RunAnimation&&fly==false&&DesiredAnimation!=RunWoundedAnimation&&HP>0.0f){
		DesiredAnimation =(HP>50.0f)?RunAnimation:RunWoundedAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if(PlayerSpeedSqr==0.0f&&DesiredAnimation!=IdleAnimation&&DesiredAnimation!=IdleWoundedAnimation&&HP>0.0f){
		DesiredAnimation = (HP>50.0f)?IdleAnimation:IdleWoundedAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if(fly==true &&DesiredAnimation != JumpAnimation&&HP>0.0f){
		DesiredAnimation = JumpAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	if(HP<=0.f&& DesiredAnimation!=DieAnimation){
		DesiredAnimation=DieAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}

}


void ABunAssistant::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABunAssistant::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetSword", IE_Released, this, &ABunAssistant::GetSword);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ABunAssistant::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABunAssistant::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABunAssistant::TouchStopped);
}

void ABunAssistant::MoveRight(float Value)
{
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void ABunAssistant::DoubleJump()
{
	fly=true;
	if (JumpCount <= 1) {
		ACharacter::LaunchCharacter(FVector(0,0,JumpHeight),false,true);
		JumpCount++;
	}
	else
	{
		Jump();
	}
}

void ABunAssistant::Landed(const FHitResult& Hit)
{
	fly = false;
	JumpCount = 0;	
}


void ABunAssistant::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	DoubleJump();
}


void ABunAssistant::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimations();
}

void ABunAssistant::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}
