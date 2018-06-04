// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MasterItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_HOME_API AMasterItem : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMasterItem();
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UItemDataTableComponent* ItemDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemCount;
};
