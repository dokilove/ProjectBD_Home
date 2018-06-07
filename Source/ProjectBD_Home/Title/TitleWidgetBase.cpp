// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"

void UTitleWidgetBase::NativeConstruct()
{
	UserID = Cast<UEditableTextBox>(GetWidgetFromName("UserID"));
	if (UserID)
	{
		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserID->SetText(FText::FromString(GI->UserID));
		}
	}
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName("ServerIP"));
	StartButton = Cast<UButton>(GetWidgetFromName("StartButton"));
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::GameStart);
	}
	ConnectButton = Cast<UButton>(GetWidgetFromName("ConnectButton"));
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ConnectServer);
	}
}

void UTitleWidgetBase::GameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidgetBase::ConnectServer()
{
	if (ServerIP)
	{
		UGameplayStatics::OpenLevel(GetWorld(), *ServerIP->GetText().ToString());
	}
}

void UTitleWidgetBase::SetUserID()
{
	if (UserID)
	{
		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			GI->UserID = UserID->GetText().ToString();
		}
	}
}

void UTitleWidgetBase::ShowLoading()
{

}