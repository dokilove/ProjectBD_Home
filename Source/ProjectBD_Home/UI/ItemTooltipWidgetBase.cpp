// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemTooltipWidgetBase.h"
#include "Components/TextBlock.h"

void UItemTooltipWidgetBase::NativeConstruct()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemName"))));
}
