// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlePC.h"
#include "Basic/MyCameraManager.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "UI/InventoryWidgetBase.h"

ABattlePC::ABattlePC()
{
	PlayerCameraManagerClass = AMyCameraManager::StaticClass();
}

void ABattlePC::BeginPlay()
{
	S2C_SetupWidget();
}

void ABattlePC::S2C_SetupWidget_Implementation()
{
	FStringClassReference ItemTooltipRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemTooltipWidget.ItemTooltipWidget_C'"));
	if (UClass* ItemTooltipClass = ItemTooltipRef.TryLoadClass<UUserWidget>())
	{
		ItemTooltip = CreateWidget<UItemTooltipWidgetBase>(this, ItemTooltipClass);
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference InventoryRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InventoryWidget.InventoryWidget_C'"));
	if (UClass* InventoryClass = InventoryRef.TryLoadClass<UUserWidget>())
	{
		Inventory = CreateWidget<UInventoryWidgetBase>(this, InventoryClass);
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABattlePC::ToggleInventory()
{
	if (Inventory->GetVisibility() == ESlateVisibility::Visible)
	{
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		Inventory->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}
