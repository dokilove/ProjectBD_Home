// Fill out your copyright notice in the Description page of Project Settings.

#include "TitlePC.h"
#include "Title/TitleWidgetBase.h"


void ATitlePC::BeginPlay()
{
	FStringClassReference TitleWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/TitleWidget.TitleWidget_C'"));
	if (UClass* TitleWidgetClass = TitleWidgetRef.TryLoadClass<UUserWidget>())
	{
		TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidget->AddToViewport();
	}

	FStringClassReference LoadingWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/LoadingWidget.LoadingWidget_C'"));
	if (UClass* LoadingWidgetClass = LoadingWidgetRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void ATitlePC::ShowLoading()
{
	LoadingWidget->SetVisibility(ESlateVisibility::Visible);
}