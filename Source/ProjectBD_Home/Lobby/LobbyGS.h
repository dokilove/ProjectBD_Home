// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_LeftTime)
	int LeftTime = 20;
	UPROPERTY(ReplicatedUsing = OnRep_UserCount)
	int UserCount;
	
	UFUNCTION()
	void OnRep_LeftTime();
	UFUNCTION()
	void OnRep_UserCount();
};
