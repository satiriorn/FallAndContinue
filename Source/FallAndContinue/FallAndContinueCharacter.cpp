#include "FallAndContinueCharacter.h"
#include "GameInstanceBun.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

AFallAndContinueCharacter::AFallAndContinueCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(30.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraBun"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	JumpHeight = 1100.f;
	moveRight = true;
	ShootTime = 0;
	Reload = 2;
	God = 0.0f;
	GodTrue = false;
	NeedShoot = false;
	GeneralCountJump = 0.0f;
	HP = 100.0f;
	TimeDeadAnim = 0.0f;
	TimeShootAnim = 0.0f;
	TimeJumpAnim = 0.0f;
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
	JumpCount = 0;
}

void AFallAndContinueCharacter::DoubleJump()
{
	if (JumpCount <= 1 && GeneralCountJump > 0) {
		ACharacter::LaunchCharacter(FVector(0,0,JumpHeight),false,true);
		JumpCount++;
		if(JumpCount>1)
			GeneralCountJump--;
	}
	else
	{
		Jump();
	}
}

void AFallAndContinueCharacter::JumpVoice()
{
	if(JumpToRight==true)
		ACharacter::LaunchCharacter(FVector(500, 0, 800), false, true);
	else
		ACharacter::LaunchCharacter(FVector(-500, 0, 800), false, true);
}

void AFallAndContinueCharacter::OffShoot()
{
	NeedShoot = false;
}

void AFallAndContinueCharacter::TimeGod()
{
	if (God>0.0f) {
		GodTrue = true;
	}
	else
	{
		GodTrue = false;
	}
}

void AFallAndContinueCharacter::Shoot()
{
	if (ShootTime<=0.0f) {
		NeedShoot = true;
		FVector location = GetActorLocation();		
		FRotator rotation = GetActorRotation();
		UWorld *WRLD = GetWorld();
		WRLD->SpawnActor(ActorToSpawn, &location, &rotation);
		ShootTime = Reload;
	}
}

void AFallAndContinueCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	static UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
	else if (JumpCount > 0 && DesiredAnimation != JumpAnimation&&TimeJumpAnim<=0.0f)
	{
		DesiredAnimation = JumpAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);
		TimeJumpAnim = 1.5;

	}
	else if (NeedShoot == true && DesiredAnimation!=ShootAnimation&& TimeShootAnim<=0.0f)
	{
		DesiredAnimation = ShootAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);
		TimeShootAnim = 1.1;
	}
	else if(HP<=0.0f&&DesiredAnimation!=FallAnimation&&TimeDeadAnim==0.0f)
	{
		DesiredAnimation = FallAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);
		TimeDeadAnim = 2.0f;
		
	}
	else if(JumpCount == 0 && NeedShoot == false && ShootTime < 1.0f && TimeDeadAnim == 0.0f && TimeShootAnim <= 0.0f&&TimeJumpAnim <= 0.0f)
	{
		DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	}

}

void AFallAndContinueCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();	
	if (ShootTime > 0.0f) 
		ShootTime -= DeltaSeconds;
	if (God > 0.0f)
		God -= DeltaSeconds;
	if (TimeDeadAnim > 0.0f) 
		TimeDeadAnim -= DeltaSeconds;
	if (TimeShootAnim > 0.0f)
		TimeShootAnim -= DeltaSeconds;
	if (TimeJumpAnim > 0.0f)
		TimeJumpAnim -= DeltaSeconds;
}

void AFallAndContinueCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFallAndContinueCharacter::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFallAndContinueCharacter::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFallAndContinueCharacter::Shoot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFallAndContinueCharacter::OffShoot);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFallAndContinueCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFallAndContinueCharacter::TouchStopped);
}

void AFallAndContinueCharacter::Landed(const FHitResult& Hit)
{
	JumpCount = 0;	
}

void AFallAndContinueCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	if (Value == 1.0f)
		moveRight = true;
	if (Value == -1.0f)
		moveRight = false;
}

void AFallAndContinueCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	DoubleJump();
}

void AFallAndContinueCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AFallAndContinueCharacter::UpdateCharacter()
{
	UpdateAnimation();
	TimeGod();
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}
