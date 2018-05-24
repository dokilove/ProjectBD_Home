// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API AMyCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
		
public:
	AMyCameraManager();

	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;

	FVector SpringArmOffset;

	float DefaultFOV;
	float NormalFOV = 90.0f;
	float IronsightFOV = 60.0f;
};
