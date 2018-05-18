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

	float CrouchOffset;
	float DefaultCameraZ;
};
