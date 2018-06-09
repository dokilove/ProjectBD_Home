// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ABattlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABattlePC();

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void S2C_SetupWidget();
	void S2C_SetupWidget_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UItemTooltipWidgetBase* ItemTooltip;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UInventoryWidgetBase* Inventory;

	UFUNCTION(BlueprintCallable)
		void ToggleInventory();
	
};
