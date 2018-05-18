// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMyHUD();
	virtual void DrawHUD() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Crosshair;
};
