// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROJECTBD_HOME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void LookUp(float Value);
	UFUNCTION()
		void Turn(float Value);
	UFUNCTION()
		void TryCrouch();
	UFUNCTION()
		void TryIronsight();
	void SetIronsight();
	void ReleaseIronsight();
	UFUNCTION()
		void TryProne();
	void StartProne();
	void EndProne();

	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void UnSprint();
	UFUNCTION()
		void LookAround();
	UFUNCTION()
		void LookForward();
	
	FRotator GetAimoffset() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ProneSpeed = 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CrouchSpeed = 160.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float JogSpeed = 360.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float SprintSpeed = 750.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsIronsight = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSprint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsProne = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsProning = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector NormalSpringArmPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CrouchSpringArmPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector ProneSpringArmPosition;


	FRotator ControllerRotataion;
};
