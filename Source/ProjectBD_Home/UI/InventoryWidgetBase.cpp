// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidgetBase.h"
#include "Components/ScrollBox.h"
#include "UI/ItemSlotWidgetBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	InventoryScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScrollBox")));

	for (int i = 0; i < InventoryScrollBox->GetChildrenCount(); ++i)
	{
		UItemSlotWidgetBase* Slot = Cast<UItemSlotWidgetBase>(InventoryScrollBox->GetChildAt(i));
		if (Slot)
		{
			Slots.Add(Slot);
		}
	}
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetEmptySlot()
{
	for (auto Slot : Slots)
	{
		if (Slot->bIsEmpty)
		{
			return Slot;
		}
	}

	return nullptr;
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetSameIDSlot(int ItemIndex)
{
	for (auto Slot : Slots)
	{
		if (Slot->Data.ItemIndex == ItemIndex && !Slot->bIsEmpty)
		{
			return Slot;
		}
	}

	return nullptr;
}
