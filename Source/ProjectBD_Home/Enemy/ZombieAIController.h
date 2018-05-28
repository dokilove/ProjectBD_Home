// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController();
	virtual void Possess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTreeComponent* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlackboardComponent* BBComponent;
};
