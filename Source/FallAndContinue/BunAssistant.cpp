#include "BunAssistant.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


using namespace std;

class USkeletalMeshSocket;

ABunAssistant::ABunAssistant()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; 

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	GetCharacterMovement()->GroundFriction = 10.0f;
	
	CameraBoom3D = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom3D"));
	CameraBoom3D->SetupAttachment(RootComponent);
	CameraBoom3D->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom3D->bDoCollisionTest = false;
	CameraBoom3D->TargetArmLength = 500.0f;
	CameraBoom3D->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom3D->RelativeRotation = FRotator(0.f, 0.f, 0.f);
	CameraBoom3D->bUsePawnControlRotation = true; 

	Camera3D = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3D"));
	Camera3D->AttachToComponent(CameraBoom3D, FAttachmentTransformRules::KeepWorldTransform);
	Camera3D->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	
	JumpCount = 0;
	JumpHeight = 1100.f;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKmodel(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/CommonerSK.CommonerSK"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>RunAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Sprint_MagicWandAnim.Sprint_MagicWandAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>RunwoundedAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Run_MagicWandAnim.Run_MagicWandAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>IdleAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Idle_MagicWandAnim.Idle_MagicWandAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>JumpAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/JumpAir_noWeaponAnim.JumpAir_noWeaponAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>IdlewoundedAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/MagicWandStance/Dizzy_MagicWandAnim.Dizzy_MagicWandAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>DieAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/Die_noWeaponAnim.Die_noWeaponAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>SelectionWeaponAnim(TEXT("/Game/ModularRPGHeroesPolyart/Animations/NoWeaponStance/PickUp_noWeaponAnim.PickUp_noWeaponAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>Tophit(TEXT("/Game/ModularRPGHeroesPolyart/Animations/TwoHandSwordStance/Combo05_SingleTwohandSwordAnim.Combo05_SingleTwohandSwordAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>Belowhit(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/NormalAttack01_SwordShieldAnim.NormalAttack01_SwordShieldAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>NormalAttacks(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/NormalAttack02_SwordShieldAnim.NormalAttack02_SwordShieldAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>Slide(TEXT("/Game/ModularRPGHeroesPolyart/Animations/SwordAndShieldStance/Slide_SwordShieldAnim.Slide_SwordShieldAnim"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence>TOPHIT(TEXT("/Game/ModularRPGHeroesPolyart/Animations/DoubleSwordStance/Combo03_DoubleSwordAnim.Combo03_DoubleSwordAnim"));
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
	SlideAnimation = Slide.Object;
	TopHit = TOPHIT.Object;
	Varibl=false;
	Health = NewObject<UHP>(UHP::StaticClass());
	HP =Health->Health;
	GetMesh()->SetSkeletalMesh(SKmodel.Object);
	TimeGetSwords = 0.0f;	
	TimeAnimationAttack = 0.0f;
	TimeAnimationSlide = 0.0f;
	EnableZoneWeapon = false;
	EnableZoneShield = false;
	SpaceState3D = false;
	SpaceState2D = true; 
	Rotate = 180.0f;
	RunSpeed = false;
	Run = false;
}
	
void ABunAssistant::StartSlide(){
	StateSlide = true;
	GetCharacterMovement()->GroundFriction = 0.1f;

}
void ABunAssistant::StopSlide(){
	GetCharacterMovement()->GroundFriction = 1.0f;
	StateSlide = false;
}
void ABunAssistant::GetSword()
{	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Sword"));
	FName fnWeaponSocket = TEXT("RightWeaponShield");
	GetSwords = true;
	Varibl = true;
	AMeleeWeapon* MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(ObjMeleeWeapon, FVector(), FRotator());
	if(MeleeWeapon){
		MeleeWeapon->Mesh->SetSimulatePhysics(false);
		MeleeWeapon->Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MeleeWeapon->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), fnWeaponSocket);
	}
}

void ABunAssistant::GetState()
{
	if(EnableZoneWeapon)
	{
		GetSwordAnimation = true;
		GetWorldTimerManager().SetTimer(InOutHandle, this, &ABunAssistant::GetSword, 1.2, false);
	}
	if(EnableZoneShield){
		GetSwordAnimation = true;
		GetWorldTimerManager().SetTimer(InOutHandle, this, &ABunAssistant::GetShield, 1.2, false);
	}
}

void ABunAssistant::GetShield()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Shield"));
	AShield* Shield; 
	FName fnWeaponSocket = TEXT("LeftWeaponShield");
	GetShields = true;
	Shield = GetWorld()->SpawnActor<AShield>(ObjShield, FVector(), FRotator());
	if(Shield){
		Shield->Mesh->SetSimulatePhysics(false);
		Shield->Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Shield->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), fnWeaponSocket);
	}
}

void ABunAssistant::SetSword(){GetSwords=false;GetSwordAnimation = false;GetShields = false;}

void ABunAssistant::MeleeAttack(){
	Attack = true;
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadTwo)){
			AttackBelow=true;
	}	
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadSix)&&GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadFour))
	{
			AttackNormal=true;
	}	
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadOne)&&GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::NumPadThree))
	{
			TopHitState=true;
	}	

}
	
void ABunAssistant::StopMeleeAttack(){Attack=false;AttackBelow=false;AttackNormal=false;TopHitState=false;}

void ABunAssistant::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->PlayAnimation(IdleAnimation, true);
}

void ABunAssistant::UpdateAnimations()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	static UAnimSequence* DesiredAnimation;
	
	if(PlayerSpeedSqr>0.0f && DesiredAnimation!= RunAnimation&& fly==false && DesiredAnimation!= RunWoundedAnimation&& HP>0.0f && TimeGetSwords<=0.0f && GetSwordAnimation !=true &&
	TimeAnimationAttack <=0.0f&&Attack==false&& StateSlide!=true&&TimeAnimationSlide<=0.0f||PlayerSpeedSqr>0.0f&& sprint==true && DesiredAnimation!= RunAnimation && HP>0.0f
	&& TimeGetSwords<=0.0f && GetSwordAnimation !=true && TimeAnimationAttack <=0.0f&&Attack==false&& StateSlide!=true&&TimeAnimationSlide<=0.0f&&fly==false){
		DesiredAnimation =(sprint)?RunAnimation:RunWoundedAnimation;
		if(ChangeCamera)
			GetMesh()->PlayAnimation(IdleAnimation, true);
		else
			GetMesh()->PlayAnimation(DesiredAnimation, true);
	}

	else if(PlayerSpeedSqr==0.0f && DesiredAnimation != IdleAnimation && DesiredAnimation != IdleWoundedAnimation && HP>0.0f && TimeGetSwords <= 0.0f&& GetSwordAnimation !=true&&
	TimeAnimationAttack <=0.0f&&AttackBelow==false&&TimeAnimationSlide<=0.0f){
		DesiredAnimation = (HP>50.0f)?IdleAnimation:IdleWoundedAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
	else if(DesiredAnimation!=WeaponSelection && TimeGetSwords<=0.0f && GetSwordAnimation !=false&&TimeAnimationSlide<=0.0f){
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
	else if(fly==false && DesiredAnimation != LowAttack && HP>0.0f && GetSwordAnimation !=true && AttackBelow==true && TimeAnimationAttack <= 0.0f &&TimeAnimationSlide<=0.0f){
		DesiredAnimation = LowAttack;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationAttack = 0.7f;
	}
	else if(fly==false && DesiredAnimation != NormalAttack && HP>0.0f && GetSwordAnimation !=true && AttackNormal==true && TimeAnimationAttack <= 0.0f &&TimeAnimationSlide<=0.0f){
		DesiredAnimation = NormalAttack;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationAttack = 1.0f;
	}
	else if(fly==false && DesiredAnimation != TopHit && HP>0.0f && GetSwordAnimation !=true && TopHitState==true && TimeAnimationAttack <= 0.0f &&TimeAnimationSlide<=0.0f){
		DesiredAnimation = TopHit;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationAttack = 1.0f;
	}
	else if(fly==false && DesiredAnimation != SlideAnimation && HP>0.0f && GetSwordAnimation !=true && StateSlide==true&&TimeAnimationSlide<=0.0f){
		DesiredAnimation = SlideAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
		TimeAnimationSlide = 0.7f;
	}
	if(HP<=0.0f && DesiredAnimation!=DieAnimation ){
		DesiredAnimation=DieAnimation;
		GetMesh()->PlayAnimation(DesiredAnimation, true);
	}
}


void ABunAssistant::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABunAssistant::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetSword", IE_Pressed, this, &ABunAssistant::GetState);
	PlayerInputComponent->BindAction("GetSword", IE_Released, this, &ABunAssistant::SetSword);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ABunAssistant::MeleeAttack);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Released, this, &ABunAssistant::StopMeleeAttack);
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &ABunAssistant::StartSlide);
	PlayerInputComponent->BindAction("Slide", IE_Released, this, &ABunAssistant::StopSlide);
	PlayerInputComponent->BindAction("ChangeSpace", IE_Pressed, this, &ABunAssistant::ChangeSpace);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABunAssistant::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABunAssistant::StopSprint);
	
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ABunAssistant::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABunAssistant::MoveForward);
	PlayerInputComponent->BindAxis("Yaw", this, &ABunAssistant::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &ABunAssistant::Pitch);
	
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABunAssistant::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABunAssistant::TouchStopped);
}



void ABunAssistant::MoveForward(float Value)
{
	if(SpaceState3D && Attack!=true && TimeGetSwords<=0.0f && Controller != NULL && Value != 0.0f)
	{	
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

}

void ABunAssistant::MoveRight(float Value)
{
	if(SpaceState3D&&ChangeCamera==false&&Attack!=true&&TimeGetSwords<=0.0f&&Controller != NULL && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
	else if(SpaceState2D&&Attack!=true&&TimeGetSwords<=0.0f&&Controller != NULL && (Value != 0.0f))	
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

void ABunAssistant::Sprint(){if(SpaceState3D){GetCharacterMovement()->MaxWalkSpeed = 825.f;sprint=true;GetMesh()->PlayAnimation(RunAnimation, true);}}
void ABunAssistant::StopSprint(){GetCharacterMovement()->MaxWalkSpeed = 600.f;sprint=false;}

void ABunAssistant::ChangeSpace(){
	if(SpaceState2D){
		SpaceState2D = false;
		ChangeCamera=true;
		SpaceState3D = true;
	}
	else{
		SpaceState3D = false;
		ChangeCamera=true;
		SpaceState2D=true;
	}
	
}
void ABunAssistant::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimations();
	SwichSpace();
	//HP= Health->Health;
	DeltaTime = DeltaSeconds;
	if(TimeGetSwords>0.0f)
		TimeGetSwords-=DeltaSeconds;
	if(TimeAnimationAttack>0.0f)
		TimeAnimationAttack-=DeltaSeconds;
	if(TimeAnimationSlide>0.0f)
		TimeAnimationSlide-=DeltaSeconds;
	if(Varibl&&Run==false){
		GetCharacterMovement()->MaxWalkSpeed =3000.0f;
		MoveForward(1.0);
		Run =true;
	}
	if(Run)
		MoveForward(1.0);
}
void ABunAssistant::SwichSpace(){
	if(SpaceState3D&&Rotate>0.0f){
		Rotate-=1.0f;
		CameraBoom->RelativeRotation = FRotator(0.f, Rotate, 0.f);
		MoveForward(0.1);
	}
	else if(SpaceState2D&&Rotate<180.0f)
	{	
		Camera3D->Deactivate();
		SideViewCameraComponent->Activate();
		Rotate+=1.0f;
		CameraBoom->RelativeRotation = FRotator(0.f, Rotate, 0.f);
		MoveRight(0.1);
	}
	else{
		ChangeCamera = false;
		if(SpaceState3D){
			Camera3D->Activate();
			SideViewCameraComponent->Deactivate();
		}
	}
			
}
	
void ABunAssistant::Yaw(float amount)
{
	if(SpaceState3D)
		AddControllerYawInput(30.f*amount * GetWorld()->GetDeltaSeconds());
}

void ABunAssistant::Pitch(float amount)
{
	if(SpaceState3D)
		AddControllerPitchInput(30.f * amount * GetWorld()->GetDeltaSeconds());
}

void ABunAssistant::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}