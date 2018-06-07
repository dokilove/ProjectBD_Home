// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ABasicPC : public APlayerController
{
	GENERATED_BODY()


public:
	ABasicPC();

	virtual void BeginPlay() override;

	void SetupWidget();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UItemTooltipWidgetBase* ItemTooltip;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UInventoryWidgetBase* Inventory;

	UFUNCTION(BlueprintCallable)
		void ToggleInventory();
};
