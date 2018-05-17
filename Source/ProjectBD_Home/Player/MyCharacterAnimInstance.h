// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Direction;
};
