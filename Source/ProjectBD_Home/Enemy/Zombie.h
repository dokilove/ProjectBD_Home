// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Chase UMETA(DisplayName = "Chase"),
	Dead UMETA(DisplayName = "Dead"),
	Battle UMETA(DisplayName = "Battle"),
};

UENUM(BlueprintType)
enum class EZombieAnimState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Wall UMETA(DisplayName = "Wall"),
	Run UMETA(DisplayName = "Run"),
	Scream UMETA(DisplayName = "Scream"),
	Death UMETA(DisplayName = "Death"),
	Attack01 UMETA(DisplayName = "Attack01"),
	Attack02 UMETA(DisplayName = "Attack02"),
};

UCLASS()
class PROJECTBD_HOME_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EZombieState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EZombieAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
};
