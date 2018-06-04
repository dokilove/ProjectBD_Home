// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UItemSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UImage* ItemThumb;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* ItemCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UButton* ItemButton;
};
