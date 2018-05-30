// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombieHPBarWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UZombieHPBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HPRatio;
	
	
};
