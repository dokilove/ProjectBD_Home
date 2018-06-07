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
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}