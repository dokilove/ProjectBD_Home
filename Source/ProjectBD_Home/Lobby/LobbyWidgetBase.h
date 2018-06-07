// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UButton* GameStartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UScrollBox* ChattingBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UEditableTextBox* ChattingInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* PublicMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextBlock* Alive;
	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void OnTextCommitted(const FText & Text, ETextCommit::Type CommitMethod);

	void HideStartButton();

};
