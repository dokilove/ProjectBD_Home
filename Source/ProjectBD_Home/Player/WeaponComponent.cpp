// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
#include "UObject/ConstructorHelpers.h"

UWeaponComponent::UWeaponComponent()
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("StaticMesh'/Game/Weapons/M4A1/SM_M4A1.SM_M4A1'"));
	if (SM_Weapon.Succeeded())
	{
		SetStaticMesh(SM_Weapon.Object);
	}
}

