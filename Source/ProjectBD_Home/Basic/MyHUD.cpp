// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Crosshair(TEXT("Texture2D'/Game/Weapons/crosshair.crosshair'"));
	if (T_Crosshair.Succeeded())
	{
		Crosshair = T_Crosshair.Object;
	}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawTextureSimple(Crosshair,
		Canvas->SizeX / 2 - Crosshair->GetSizeX() / 2,
		Canvas->SizeY / 2 - Crosshair->GetSizeY() / 2);
}
