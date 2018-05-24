// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 50, 70);
	SpringArm->TargetArmLength = 180.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Male(TEXT("SkeletalMesh'/Game/Male_Grunt/Mesh/male_grunt.male_grunt'"));
	if (SK_Male.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Male.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	FStringClassReference Anim_Male_Ref(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Male.ABP_Male_C'"));
	if (UClass* Anim_Male_Class = Anim_Male_Ref.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Male_Class);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("StaticMesh'/Game/Weapons/M4A1/SM_M4A1.SM_M4A1'"));
	if (SM_Weapon.Succeeded())
	{
		Weapon->SetStaticMesh(SM_Weapon.Object);
	}

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity.Size() <= 0.0f && !bIsProne)
	{
		UnSprint();
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,
		&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,
		&AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,
		&AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this,
		&AMyCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryIronsight);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::TryProne);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this,
		&AMyCharacter::UnSprint);
	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Pressed, this,
		&AMyCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Released, this,
		&AMyCharacter::LookForward);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bIsProning)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsSprint && !bIsProning)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void AMyCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value);
	}
}
void AMyCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void AMyCharacter::TryCrouch()
{
	if (bIsProne)
	{
		return;
	}

	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::TryIronsight()
{
	if (bIsProne)
	{
		return;
	}

	if (!bIsIronsight)
	{
		SetIronsight();
	}
	else
	{
		ReleaseIronsight();
	}
}

void AMyCharacter::SetIronsight()
{
	bIsIronsight = true;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed / 2;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed / 2;
}

void AMyCharacter::ReleaseIronsight()
{
	bIsIronsight = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

void AMyCharacter::TryProne()
{
	if (!bIsProne)
	{
		StartProne();
	}
	else
	{
		EndProne();
	}
	UnCrouch();
}

void AMyCharacter::StartProne()
{
	bIsProne = true;

	GetCharacterMovement()->MaxWalkSpeed = ProneSpeed;
}

void AMyCharacter::EndProne()
{
	bIsProne = false;
	if (bIsIronsight)
	{
		SetIronsight();
	}
	else
	{
		ReleaseIronsight();
	}
}

void AMyCharacter::Sprint()
{
	if (!bIsCrouched && !bIsIronsight)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::UnSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void AMyCharacter::LookAround()
{
	if (!bIsIronsight)
	{
		bUseControllerRotationYaw = false;
		ControllerRotataion = GetController()->GetControlRotation();
		//UE_LOG(LogClass, Warning, TEXT("Rotation %s"), *(ControllerRotataion.ToString()));
	}
}

void AMyCharacter::LookForward()
{
	bUseControllerRotationYaw = true;
	GetController()->SetControlRotation(ControllerRotataion);
}

FRotator AMyCharacter::GetAimoffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRot = AimDirLS.Rotation();
	return AimRot;
}
