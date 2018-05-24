// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraManager.h"
#include "Player/MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"


AMyCameraManager::AMyCameraManager()
{

}

void AMyCameraManager::BeginPlay()
{
	Super::BeginPlay();

	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		DefaultFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
		SpringArmOffset = Pawn->SpringArm->GetRelativeTransform().GetLocation();
	}
}

void AMyCameraManager::UpdateCamera(float DeltaTime)
{
	FVector TargetOffset;
	float TargetFOV = 0.0f;
	Super::UpdateCamera(DeltaTime);

	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 10.0f);
		SetFOV(DefaultFOV);

		if (Pawn->bIsProne)
		{
			TargetOffset = Pawn->ProneSpringArmPosition;
		}
		else if (Pawn->bIsCrouched)
		{
			TargetOffset = Pawn->CrouchSpringArmPosition;
		}
		else
		{
			TargetOffset = Pawn->NormalSpringArmPosition;
		}

		SpringArmOffset = FMath::VInterpTo(SpringArmOffset, TargetOffset, DeltaTime, 5.0f);
		Pawn->SpringArm->SetRelativeLocation(SpringArmOffset);
	}
}
