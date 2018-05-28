// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectPatrolPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Zombie.h"
#include "Enemy/PatrolPoint.h"

EBTNodeResult::Type UBTTask_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		APatrolPoint* PatrolPoint = Zombie->PatrolPoints[FMath::RandRange(0, Zombie->PatrolPoints.Num() - 1)];
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PatrolPoint);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
