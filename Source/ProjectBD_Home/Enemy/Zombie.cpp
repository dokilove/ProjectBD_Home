// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/PatrolPoint.h"


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
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		CurrentState = EZombieState::Dead;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->SetSimulatePhysics(true);
	}

	return 0.0f;
}

