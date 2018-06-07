// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	class UTitleWidgetBase* TitleWidget;
};
