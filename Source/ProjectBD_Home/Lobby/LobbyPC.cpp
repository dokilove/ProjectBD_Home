// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"

void ALobbyPC::BeginPlay()
{
	S2C_SetupWidget();
	SetInputMode(FInputModeGameAndUI());
}

void ALobbyPC::S2C_SetupWidget_Implementation()
{
	FStringClassReference LobbyWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/LobbyWidget.LobbyWidget_C'"));
	if (UClass* LobbyWidgetClass = LobbyWidgetRef.TryLoadClass<UUserWidget>())
	{
		LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		LobbyWidget->AddToViewport();
		if (!HasAuthority())
		{
			LobbyWidget->HideStartButton();
		}
	}

	FStringClassReference LoadingWidgetRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/LoadingWidget.LoadingWidget_C'"));
	if (UClass* LoadingWidgetClass = LoadingWidgetRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPC::GameStart()
{
	for (auto i = GetWorld()->GetControllerIterator(); i; ++i)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*i);
		PC->S2C_ShowLoading();
	}
	GetWorld()->ServerTravel(TEXT("Battle"));
}


void ALobbyPC::S2C_ShowLoading_Implementation()
{
	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}