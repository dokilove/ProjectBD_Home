// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieWalk.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieWalk::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
	}
}

void UAnimNotifyState_ZombieWalk::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = 3.0f;
	}
}
