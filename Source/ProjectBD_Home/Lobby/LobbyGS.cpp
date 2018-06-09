// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGS.h"
#include "UnrealNetwork.h"
#include "Lobby/LobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lobby/LobbyWidgetBase.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, UserCount);
}

void ALobbyGS::OnRep_LeftTime()
{
	//UE_LOG(LogClass, Warning, TEXT("OnRep_LeftTime %d"), LeftTime);
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		if (LeftTime < 0)
		{
			if (HasAuthority())
			{
				PC->GameStart();
			}
		}
		else
		{
			PC->LobbyWidget->SetPublicMessageText(LeftTime);
		}
	}
}

void ALobbyGS::OnRep_UserCount()
{
	//UE_LOG(LogClass, Warning, TEXT("OnRep_UserCount %d"), UserCount);
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetAliveText(UserCount);
	}
}
