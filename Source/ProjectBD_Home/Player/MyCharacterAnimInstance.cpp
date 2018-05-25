// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterAnimInstance.h"
#include "Player/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());

	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity,
			Pawn->GetActorRotation());
		bIsCrouch = Pawn->bIsCrouched;
		Pitch = Pawn->GetAimoffset().Pitch;
		bIsIronsight = Pawn->bIsIronsight;
		bIsSprint = Pawn->bIsSprint;
		bIsProne = Pawn->bIsProne;
		bIsProning = Pawn->bIsProning;
		bIsFire = Pawn->bIsFire;
	}
}

void UMyCharacterAnimInstance::AnimNotify_LockMove()
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());

	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsProning = true;
	}
}

void UMyCharacterAnimInstance::AnimNotify_UnlockMove()
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());

	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsProning = false;
	}
}
