// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckBattle.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Zombie.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/MyCharacter.h"

EBTNodeResult::Type UBTTask_CheckBattle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
		if (Player && Player->IsValidLowLevelFast())
		{
			float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
			FVector Dir = Player->GetActorLocation() - Zombie->GetActorLocation();
			Zombie->SetActorRotation(Dir.Rotation());

			if (Range > Zombie->AttackRange)
			{
				Zombie->CurrentState = EZombieState::Chase;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
			else if (Player->CurrentHP <= 0.0f || Range > Zombie->PawnSensing->SightRadius)
			{
				Zombie->CurrentState = EZombieState::Normal;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
