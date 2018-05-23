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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool bIsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool bIsIronsight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool bIsSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool bIsProne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool bIsProning;

	UFUNCTION(BlueprintCallable)
		void AnimNotify_LockMove();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_UnlockMove();
};
