// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPC.h"
#include "BDGameInstance.h"

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
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->GameStart();
	}
}

void ULobbyWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if (CommitMethod == ETextCommit::OnEnter)
		{
			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI)
			{
				FString Message = FString::Printf(TEXT("%s : %s"), *GI->UserID, *Text.ToString());
				PC->C2S_ChattingInput(Message);

				ChattingInput->SetText(FText::FromString(TEXT("")));
			}
		}
		else if (CommitMethod == ETextCommit::OnCleared)
		{
			ChattingInput->SetUserFocus(PC);
		}
	}
}

void ULobbyWidgetBase::HideStartButton()
{
	GameStartButton->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidgetBase::AddChatting(FString Message)
{
	if (ChattingBox)
	{
		UTextBlock* Text = NewObject<UTextBlock>();
		Text->SetText(FText::FromString(Message));
		ChattingBox->AddChild(Text);
		ChattingBox->ScrollToEnd();
	}
}
