// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGM.h"
#include "UObject/ConstructorHelpers.h"
#include "Basic/BasicPC.h"
#include "Player/MyCharacter.h"
#include "Basic/MyHUD.h"

ABasicGM::ABasicGM()
{
	static ConstructorHelpers::FClassFinder<ABasicPC> PC_Class(TEXT("Blueprint'/Game/Blueprints/BP_BasicPC.BP_BasicPC_C'"));
	if (PC_Class.Succeeded())
	{
		PlayerControllerClass = PC_Class.Class;
	}

	static ConstructorHelpers::FClassFinder<AMyCharacter> Pawn_Class(TEXT("Blueprint'/Game/Blueprints/BP_Male.BP_Male_C'"));
	if (Pawn_Class.Succeeded())
	{
		DefaultPawnClass = Pawn_Class.Class;
	}

	HUDClass = AMyHUD::StaticClass();
}

