// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraManager.h"
#include "Player/MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"


AMyCameraManager::AMyCameraManager()
{

}

void AMyCameraManager::BeginPlay()
{
	Super::BeginPlay();

	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		DefaultCameraZ = Pawn->Camera->GetRelativeTransform().GetLocation().Z;
	}
}

void AMyCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		float TargetOffset = Pawn->bIsCrouched ? Pawn->CrouchedEyeHeight : 0;
		CrouchOffset = FMath::FInterpTo(CrouchOffset, TargetOffset, DeltaTime, 5.0f);

		FVector CameraLocation = Pawn->Camera->GetRelativeTransform().GetLocation();
		FVector NewCameraLocation = FVector(CameraLocation.X, CameraLocation.Y,
			DefaultCameraZ - CrouchOffset);

		Pawn->Camera->SetRelativeLocation(NewCameraLocation);
	}
}
