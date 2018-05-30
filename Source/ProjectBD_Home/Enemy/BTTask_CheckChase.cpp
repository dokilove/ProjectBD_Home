// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckChase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Zombie.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/MyCharacter.h"

EBTNodeResult::Type UBTTask_CheckChase::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (Zombie && Zombie->IsValidLowLevelFast())
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
		if (Player && Player->IsValidLowLevelFast())
		{
			float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
			if (Range <= Zombie->AttackRange)
			{
				Zombie->CurrentState = EZombieState::Battle;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
			else if (Range > Zombie->PawnSensing->SightRadius)
			{
				Zombie->CurrentState = EZombieState::Normal;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
				return EBTNodeResult::Failed;
			}
			else
			{
				FVector TraceStart = Zombie->GetActorLocation();
				FVector TraceEnd = Player->GetActorLocation();

				TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
				TArray<AActor*> ActorsToIgnore;
				FHitResult OutHit;

				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
				ActorsToIgnore.Add(Zombie);

				bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

				if (Result)
				{
					if (!OutHit.GetActor()->ActorHasTag(FName("Player")))
					{
						Zombie->CurrentState = EZombieState::Normal;
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);
						return EBTNodeResult::Failed;
					}
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
