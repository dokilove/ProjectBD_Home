// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/Zombie.h"

AZombieAIController::AZombieAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	AZombie* Zombie = Cast<AZombie>(InPawn);
	if (Zombie && Zombie->IsValidLowLevelFast())
	{

		BBComponent->InitializeBlackboard(*Zombie->BehaviorTree->BlackboardAsset);
		BehaviorTree->StartTree(*Zombie->BehaviorTree);
	}
}
