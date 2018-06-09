// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
		void S2C_SetupWidget();
	void S2C_SetupWidget_Implementation();

	void GameStart();

	UFUNCTION(Client, Reliable)
	void S2C_ShowLoading();
	void S2C_ShowLoading_Implementation();

	class ULobbyWidgetBase* LobbyWidget;
	class UUserWidget* LoadingWidget;

	FTimerHandle StartTimer;
	void BattleStart();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_ChattingInput(const FString& Message);
	bool C2S_ChattingInput_Validate(const FString& Message);
	void C2S_ChattingInput_Implementation(const FString& Message);

	UFUNCTION(Client, Reliable)
	void S2C_AddChatting(const FString& Message);
	void S2C_AddChatting_Implementation(const FString& Message);

};
