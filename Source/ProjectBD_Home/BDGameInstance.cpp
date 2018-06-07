// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"




void UBDGameInstance::Init()
{
	UserID = FString::Printf(TEXT("Noname%d"), FMath::RandRange(1, 100));
}