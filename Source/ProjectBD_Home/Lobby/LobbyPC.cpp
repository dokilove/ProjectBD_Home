// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "TimerManager.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

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

		LobbyWidget->ChattingInput->SetUserFocus(this);
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
	if (HasAuthority())
	{
		for (auto i = GetWorld()->GetControllerIterator(); i; ++i)
		{
			ALobbyPC* PC = Cast<ALobbyPC>(*i);
			PC->S2C_ShowLoading();
		}
	}
	GetWorldTimerManager().SetTimer(StartTimer, this, &ALobbyPC::BattleStart, 1.0f, false);
}


void ALobbyPC::S2C_ShowLoading_Implementation()
{
	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALobbyPC::BattleStart()
{
	GetWorld()->ServerTravel(TEXT("Battle"));
}

void ALobbyPC::C2S_ChattingInput_Implementation(const FString& Message)
{
	if (HasAuthority())
	{
		for (auto i = GetWorld()->GetControllerIterator(); i; ++i)
		{
			ALobbyPC* PC = Cast<ALobbyPC>(*i);
			PC->S2C_AddChatting(Message);
		}
	}
}

void ALobbyPC::S2C_AddChatting_Implementation(const FString & Message)
{
	//UE_LOG(LogClass, Warning, TEXT("%s"), *Message);
	if (LobbyWidget)
	{
		LobbyWidget->AddChatting(Message);
	}
}

bool ALobbyPC::C2S_ChattingInput_Validate(const FString & Message)
{
	return true;
}
