// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Zombie.h"

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		Zombie->CurrentState = NewZombieState;
		Zombie->CurrentAnimState = NewZombieAnimState;

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
