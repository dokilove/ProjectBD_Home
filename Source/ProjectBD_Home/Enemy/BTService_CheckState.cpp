// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Zombie.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
							}
						}
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
