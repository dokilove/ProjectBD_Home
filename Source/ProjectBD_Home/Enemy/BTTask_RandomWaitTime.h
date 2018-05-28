// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomWaitTime.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API UBTTask_RandomWaitTime : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, meta = (MinClamp = 0.0, MaxClamp = 3.0, UIMin = 0.0, UIMax = 3.0))
		float MinTime;
	UPROPERTY(EditAnywhere, meta = (MinClamp = 0.0, MaxClamp = 3.0, UIMin = 0.0, UIMax = 3.0))
		float MaxTime;

};
