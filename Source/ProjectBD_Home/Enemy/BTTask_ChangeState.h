// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy/Zombie.h"
#include "BTTask_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UBTTask_ChangeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		EZombieState NewZombieState;
	UPROPERTY(EditAnywhere)
		EZombieAnimState NewZombieAnimState;

	
};