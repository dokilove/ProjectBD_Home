// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{

		switch (Zombie->CurrentState)
		{
		case EZombieState::Normal:
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
			break;
		case EZombieState::Chase:
			Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
			break;
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
