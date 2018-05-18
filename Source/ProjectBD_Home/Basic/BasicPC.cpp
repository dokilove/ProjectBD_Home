// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/MyCameraManager.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = AMyCameraManager::StaticClass();
}


