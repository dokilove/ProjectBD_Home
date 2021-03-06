// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/PatrolPoint.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/UI/ZombieHPBarWidgetBase.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("SkeletalMesh'/Game/Zombie/Mesh/Zombie.Zombie'"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	FStringClassReference AnimRef(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Zombie.ABP_Zombie_C'"));
	if (UClass* AnimClass = AnimRef.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass);
	}

	AIControllerClass = AZombieAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Zombie(TEXT("BehaviorTree'/Game/Blueprints/AI/BT_Zombie.BT_Zombie'"));
	if (BT_Zombie.Succeeded())
	{
		BehaviorTree = BT_Zombie.Object;
	}

	CurrentState = EZombieState::Normal;
	CurrentAnimState = EZombieAnimState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1300.0f;
	PawnSensing->SetPeripheralVisionAngle(60.0f);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);

	FStringClassReference WidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ZombieHPBarWidget.ZombieHPBarWidget_C'"));
	if (UClass* WidgetClass = WidgetRef.TryLoadClass<UZombieHPBarWidgetBase>())
	{
		Widget->SetWidgetClass(WidgetClass);
	}
	Widget->SetRelativeLocation(FVector(0, 0, 120));
	Widget->SetDrawSize(FVector2D(100, 30));
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), OutActors);
	for (int i = 0; i < OutActors.Num(); ++i)
	{
		PatrolPoints.Add(Cast<APatrolPoint>(OutActors[i]));
	}

	PawnSensing->OnSeePawn.AddDynamic(this, &AZombie::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AZombie::OnHearNoise);
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Dir = CameraLocation - Widget->GetComponentLocation();
	Widget->SetWorldRotation(Dir.Rotation());

	UZombieHPBarWidgetBase* HPBar = Cast<UZombieHPBarWidgetBase>(Widget->GetUserWidgetObject());
	if (HPBar)
	{
		HPBar->HPRatio = CurrentHP / MaxHP;
	}

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentState == EZombieState::Dead)
	{
		return 0.0f;
	}

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
	}

	CurrentHP -= Damage;
	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->SetSimulatePhysics(true);
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->IsValidLowLevelFast())
		{
			CurrentState = EZombieState::Dead;

			AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);

			Widget->SetVisibility(false);
		}
	}

	return 0.0f;
}

void AZombie::OnSeePawn(APawn* Pawn)
{
	AMyCharacter* Player = Cast<AMyCharacter>(Pawn);
	if (Player && Player->IsValidLowLevelFast() && CurrentState == EZombieState::Normal && Player->CurrentHP > 0.0f)
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->IsValidLowLevelFast())
		{
			CurrentState = EZombieState::Chase;

			AIC->BBComponent->SetValueAsObject(FName(TEXT("Target")), Player);
			AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
	}
}

void AZombie::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{

}

void AZombie::OnAttack()
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->IsValidLowLevelFast())
	{
		AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Target"))));
		if (Player && Player->IsValidLowLevelFast())
		{
			UGameplayStatics::ApplyDamage(Player, AttackDamage, AIC, this, nullptr);
		}
	}
}
