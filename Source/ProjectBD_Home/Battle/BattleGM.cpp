// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/ItemPoint.h"
#include "Engine/World.h"
#include "Item/MasterItem.h"

void ABattleGM::BeginPlay()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), ItemPoints);

	FStringClassReference MasterItemRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_MasterItem.BP_MasterItem_C'"));
	if (UClass* MasterItemClass = MasterItemRef.TryLoadClass<AMasterItem>())
	{
		for (auto ItemPoint : ItemPoints)
		{
			GetWorld()->SpawnActor<AMasterItem>(MasterItemClass, ItemPoint->GetActorLocation(), ItemPoint->GetActorRotation());
		}
	}
}

