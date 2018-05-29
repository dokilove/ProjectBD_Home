// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Zombie.h"
#include "Perception/PawnSensingComponent.h"

void UBTService_CheckState::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
		if (Player && Player->IsValidLowLevelFast())
		{
			float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

			switch (Zombie->CurrentState)
			{
			case EZombieState::Chase:
				{
					if (Range <= Zombie->AttackRange)
					{
						Zombie->CurrentState = EZombieState::Battle;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
					else if (Range > Zombie->PawnSensing->SightRadius)
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
				}
				break;

			case EZombieState::Battle:
				{
					if (Range > Zombie->PawnSensing->SightRadius)
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
					}
					break;
				}
			}
		}
	}
}
