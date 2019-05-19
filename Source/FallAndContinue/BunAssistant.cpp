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
	ConstructorHelpers::FObjectFinder<UAnimSequence>Tophit(TEXT("/Game/ModularRPGHeroesPolyart/Animations/TwoHandSwordStance/Combo05_SingleTwohandSwordAnim.Combo05_SingleTwohandSwordAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>Belowhit(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/NormalAttack01_SwordShieldAnim.NormalAttack01_SwordShieldAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>NormalAttacks(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/NormalAttack02_SwordShieldAnim.NormalAttack02_SwordShieldAnim"));
	ConstructorHelpers::FObjectFinder<UAnimSequence>Slide(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/Slide_SwordShieldAnim.Slide_SwordShieldAnim"));
	
	JumpAnimation =JumpAnim.Object;
	RunAnimation = RunAnim.Object;
	RunWoundedAnimation = RunwoundedAnim.Object;
	IdleWoundedAnimation = IdlewoundedAnim.Object;
	IdleAnimation = IdleAnim.Object;
	DieAnimation = DieAnim.Object;
	WeaponSelection = SelectionWeaponAnim.Object;
	JumpAttack = Tophit.Object; 
	LowAttack = Belowhit.Object;
	NormalAttack = NormalAttacks.Object;
	SlideAnimation = Slide.ObjMeleeWeapon;
	Varibl=false;
	
	HP=112.0f;
	GetMesh()->SetSkeletalMesh(SKmodel.Object);
	TimeGetSwords = 0.0f;	
	TimeAnimationAttack = 0.0f;
	EnableZoneWeapon = false;
	
}

	

void ABunAssistant::GetSword()
{	
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "monster: no bullet actor could be spawned. is the bullet overlapping something ? " ); 
	GetSwords = true;
	FName fnWeaponSocket = TEXT("RightWeaponShield");
	AMeleeWeapon* MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(ObjMeleeWeapon, FVector(), FRotator());
	if(MeleeWeapon){
		MeleeWeapon->Mesh->SetSimulatePhysics(false);
		MeleeWeapon->Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MeleeWeapon->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), fnWeaponSocket);
	}
}

	
void ABunAssistant::AnimationState(){
	if(EnableZoneWeapon)
	{
		GetSwordAnimation = true;
		GetWorldTimerManager().SetTimer(InOutHandle, this, &ABunAssistant::GetSword, 1.2, false);
	}
}

void ABunAssistant::SetSword(){GetSwords=false;GetSwordAnimation = false;}

void ABunAssistant::MeleeAttack(){
	Attack = true;
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadTwo)){
			AttackBelow=true;
	}	
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadSix)&&GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadFour){
			AttackNormal=true;
	}	
}
	
void ABunAssistant::StopMeleeAttack(){Attack=false;AttackBelow=false;}

void ABunAssistant::BeginPlay()
{
	Super::BeginPlay();
}

void ABunAssistant::UpdateAnimations()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	static UAnimSequence* DesiredAnimation;
	
	if(PlayerSpeedSqr>0.0f && DesiredAnimation!= RunAnimation&& fly==false && DesiredAnimation!= RunWoundedAnimation&& HP>0.0f && TimeGetSwords<=0.0f && GetSwordAnimation !=true &&
	TimeAnimationAttack <=0.0f&&Attack==false&& AttackBelow==false){
		DesiredAnimation =(HP>50.0f)?RunAnimation:RunWoundedAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}

	else if(PlayerSpeedSqr==0.0f && DesiredAnimation != IdleAnimation && DesiredAnimation != IdleWoundedAnimation && HP>0.0f && TimeGetSwords <= 0.0f&& GetSwordAnimation !=true&&
	TimeAnimationAttack <=0.0f&&AttackBelow==false){
		DesiredAnimation = (HP>50.0f)?IdleAnimation:IdleWoundedAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if(DesiredAnimation!=WeaponSelection && TimeGetSwords<=0.0f && GetSwordAnimation !=false){
		DesiredAnimation=WeaponSelection;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeGetSwords = 1.4f;
	}
	else if(DesiredAnimation!=JumpAttack && GetSwordAnimation !=true && TimeAnimationAttack <= 0.0f && Attack==true && HP>0.0f&& JumpCount>=1){
		DesiredAnimation = JumpAttack;
		TimeAnimationAttack = 1.15f;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if( fly==true && DesiredAnimation != JumpAnimation && HP>0.0f && GetSwordAnimation !=true && TimeAnimationAttack <= 0.0f&&Attack==false){
		DesiredAnimation = JumpAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if(fly==false && DesiredAnimation != LowAttack && HP>0.0f && GetSwordAnimation !=true && AttackBelow==true && TimeAnimationAttack <= 0.0f ){
		DesiredAnimation = LowAttack;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationAttack = 0.7f;
	}
	else if(fly==false && DesiredAnimation != NormalAttack && HP>0.0f && GetSwordAnimation !=true && AttackNormal==true && TimeAnimationAttack <= 0.0f ){
		DesiredAnimation = NormalAttack;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationAttack = 0.7f;
	}
	if(HP<=0.f && DesiredAnimation!=DieAnimation ){
		DesiredAnimation=DieAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
}


void ABunAssistant::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABunAssistant::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetSword", IE_Pressed, this, &ABunAssistant::AnimationState);
	PlayerInputComponent->BindAction("GetSword", IE_Released, this, &ABunAssistant::SetSword);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ABunAssistant::MeleeAttack);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Released, this, &ABunAssistant::StopMeleeAttack);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABunAssistant::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABunAssistant::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABunAssistant::TouchStopped);
}

void ABunAssistant::MoveRight(float Value)
{
	if(Attack!=true&&TimeGetSwords<=0.0f)	
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
	if(TimeGetSwords>0.0f)
		TimeGetSwords-=DeltaSeconds;
	if(TimeAnimationAttack>0.0f)
		TimeAnimationAttack-=DeltaSeconds;
	 //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TimeGetSwords));
}

void ABunAssistant::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}
