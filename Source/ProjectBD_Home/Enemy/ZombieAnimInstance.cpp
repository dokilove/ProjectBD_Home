// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"
#include "Enemy/Zombie.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AZombie* Zombie = Cast<AZombie>(TryGetPawnOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		CurrentState = Zombie->CurrentState;
		CurrentAnimState = Zombie->CurrentAnimState;
	}
}

void UZombieAnimInstance::AnimNotify_OnAttack()
{
	AZombie* Zombie = Cast<AZombie>(TryGetPawnOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->OnAttack();
	}
}
