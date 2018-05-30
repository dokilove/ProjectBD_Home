// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/Zombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EZombieState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EZombieAnimState CurrentAnimState;

	UFUNCTION()
		void AnimNotify_OnAttack();
};
