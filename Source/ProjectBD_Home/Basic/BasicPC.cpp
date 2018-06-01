// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/MyCameraManager.h"
#include "UI/ItemTooltipWidgetBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = AMyCameraManager::StaticClass();
}

void ABasicPC::BeginPlay()
{
	Super::BeginPlay();

	FStringClassReference ItemTooltipRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemTooltipWidget.ItemTooltipWidget_C'"));
	if (UClass* ItemTooltipClass = ItemTooltipRef.TryLoadClass<UUserWidget>())
	{
		ItemTooltip = CreateWidget<UItemTooltipWidgetBase>(this, ItemTooltipClass);
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}
