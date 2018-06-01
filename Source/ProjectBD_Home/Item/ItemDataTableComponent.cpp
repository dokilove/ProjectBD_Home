// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDataTableComponent.h"
#include "ConstructorHelpers.h"

// Sets default values for this component's properties
UItemDataTableComponent::UItemDataTableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemDataTable(TEXT("DataTable'/Game/Blueprints/Data/ItemTable.ItemTable'"));
	if (DT_ItemDataTable.Succeeded())
	{
		DataTable = DT_ItemDataTable.Object;
	}
}


// Called when the game starts
void UItemDataTableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemDataTableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FItemDataTable & UItemDataTableComponent::GetItemData(int index)
{
	// TODO: insert return statement here
	return *DataTable->FindRow<FItemDataTable>(*FString::FromInt(index), TEXT("ItemIndex"), true);
}

