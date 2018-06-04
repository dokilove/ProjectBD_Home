// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UScrollBox* InventoryScrollBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray <class UItemSlotWidgetBase*> Slots;

	UFUNCTION(BlueprintCallable)
		UItemSlotWidgetBase* GetEmptySlot();
	UFUNCTION(BlueprintCallable)
		UItemSlotWidgetBase* GetSameIDSlot(int ItemIndex);
};
