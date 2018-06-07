// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"


void ULobbyWidgetBase::NativeConstruct()
{
	Alive = Cast<UTextBlock>(GetWidgetFromName("Alive"));
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName("PublicMessage"));
	ChattingBox = Cast<UScrollBox>(GetWidgetFromName("ChattingBox"));
	ChattingInput = Cast<UEditableTextBox>(GetWidgetFromName("ChattingInput"));
	if (ChattingInput)
	{
		ChattingInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommitted);
	}
	GameStartButton = Cast<UButton>(GetWidgetFromName("GameStartButton"));
	if (GameStartButton)
	{
		GameStartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::GameStart);
	}
}

void ULobbyWidgetBase::GameStart()
{

}

void ULobbyWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	
}

void ULobbyWidgetBase::HideStartButton()
{
	GameStartButton->SetVisibility(ESlateVisibility::Collapsed);
}