// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGM.h"
#include "Lobby/LobbyGS.h"
#include "TimerManager.h"

void ALobbyGM::BeginPlay()
{
	GetWorldTimerManager().SetTimer(LeftTimerHandle, this, &ALobbyGM::StartTimer, 1.0f, true);
}

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->UserCount = GS->PlayerArray.Num();
		if (HasAuthority())
		{
			GS->OnRep_UserCount();
		}
	}
}

void ALobbyGM::StartTimer()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
		if (HasAuthority())
		{
			GS->OnRep_LeftTime();

			if (GS->LeftTime < 0)
			{
				GetWorldTimerManager().ClearTimer(LeftTimerHandle);
			}
		}
	}
}
