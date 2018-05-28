// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RandomWaitTime.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_RandomWaitTime::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	float WaitTime = FMath::FRandRange(MinTime, MaxTime);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), WaitTime);

	return EBTNodeResult::Succeeded;
}
