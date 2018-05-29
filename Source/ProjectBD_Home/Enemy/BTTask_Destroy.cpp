// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Destroy.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_Destroy::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetAIOwner()->GetPawn()->Destroy();
	return EBTNodeResult::Succeeded;
}


