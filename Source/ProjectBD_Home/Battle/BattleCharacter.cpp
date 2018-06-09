// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Player/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RifleCameraShake.h"
#include "Player/BulletDamageType.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Item/MasterItem.h"
#include "Components/TextBlock.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "Item/ItemDataTableComponent.h"
#include "Battle/BattlePC.h"
#include "UI/InventoryWidgetBase.h"
#include "UI/ItemSlotWidgetBase.h"

// Sets default values
ABattleCharacter::ABattleCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0, 50, 70));
	SpringArm->TargetArmLength = 180.0f;
	NormalSpringArmPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringArmPosition = FVector(NormalSpringArmPosition.X, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 40);
	ProneSpringArmPosition = FVector(130, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 100);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Male(TEXT("SkeletalMesh'/Game/Male_Grunt/Mesh/male_grunt.male_grunt'"));
	if (SK_Male.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Male.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	FStringClassReference Anim_Male_Ref(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_Male.ABP_Male_C'"));
	if (UClass* Anim_Male_Class = Anim_Male_Ref.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Male_Class);
	}

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_MuzzleFireEffect(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	if (P_MuzzleFireEffect.Succeeded())
	{
		MuzzleFireEffect = P_MuzzleFireEffect.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HitEffect(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (P_HitEffect.Succeeded())
	{
		HitEffect = P_HitEffect.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> S_FireSound(TEXT("SoundCue'/Game/Sound/Weapons/SMG_Thompson/Cue_Thompson_Shot.Cue_Thompson_Shot'"));
	if (S_FireSound.Succeeded())
	{
		FireSound = S_FireSound.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim_Dead(TEXT("AnimMontage'/Game/Male_Grunt/Animations/AnimMontage/Death_1_Montage.Death_1_Montage'"));
	if (Anim_Dead.Succeeded())
	{
		DeadAnimation = Anim_Dead.Object;
	}

	Tags.Add(FName(TEXT("Player")));
}

// Called when the game starts or when spawned
void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;

	CurrentHP = MaxHP;
}

// Called every frame
void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity.Size() <= 0.0f && !bIsProne)
	{
		UnSprint();
	}
}

// Called to bind functionality to input
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,
		&ABattleCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,
		&ABattleCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,
		&ABattleCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this,
		&ABattleCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::TryIronsight);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::TryProne);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::Inventory);
	PlayerInputComponent->BindAction(TEXT("GetItem"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::GetItem);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this,
		&ABattleCharacter::UnSprint);
	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), EInputEvent::IE_Released, this,
		&ABattleCharacter::LookForward);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this,
		&ABattleCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this,
		&ABattleCharacter::StopFire);
}

float ABattleCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// RadialDamage를 적용시키기 위해서는 부모함수에서 실행시켜서 계산된 대미지를 가져와야한다
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;

		//for (int32 i = 0; i < RadialDamageEvent->ComponentHits.Num(); ++i)
		//{
		//	//UE_LOG(LogClass, Warning, TEXT("location %s, Origin %s"), *RadialDamageEvent->ComponentHits[i].Location.ToString(), *RadialDamageEvent->Origin.ToString());
		//	float distance = FVector::Distance(RadialDamageEvent->ComponentHits[i].Location , RadialDamageEvent->Origin);
		//	float scale = RadialDamageEvent->Params.GetDamageScale(distance);
		//	//UE_LOG(LogClass, Warning, TEXT("ComponentHits %d %f %f %s"), i, distance, scale, *RadialDamageEvent->ComponentHits[i].GetActor()->GetName());
		//	Damage *= scale;
		//}


		UE_LOG(LogClass, Warning, TEXT("FRadialDamageEvent %f"), Damage);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		UE_LOG(LogClass, Warning, TEXT("FPointDamageEvent %f %s"), Damage, *PointDamageEvent->HitInfo.BoneName.ToString());

	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		UE_LOG(LogClass, Warning, TEXT("Damage %f"), Damage);
	}

	CurrentHP -= Damage;
	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0;

		//GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DisableInput(Cast<APlayerController>(GetController()));

		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeadAnimation))
		{
			PlayAnimMontage(DeadAnimation);
		}
	}

	return 0.0f;
}

void ABattleCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bIsProning)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void ABattleCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsSprint && !bIsProning)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void ABattleCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value);
	}
}
void ABattleCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void ABattleCharacter::TryCrouch()
{
	if (bIsProning)
	{
		return;
	}

	if (CanCrouch())
	{
		Crouch();
		if (bIsProne)
		{
			EndProne();
		}
	}
	else
	{
		UnCrouch();
	}
}

void ABattleCharacter::TryIronsight()
{
	if (bIsProning)
	{
		return;
	}

	if (!bIsIronsight)
	{
		SetIronsight();
	}
	else
	{
		ReleaseIronsight();
	}
}

void ABattleCharacter::SetIronsight()
{
	bIsIronsight = true;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed / 2;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed / 2;
}

void ABattleCharacter::ReleaseIronsight()
{
	bIsIronsight = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

void ABattleCharacter::TryProne()
{
	if (!bIsProne)
	{
		StartProne();
	}
	else
	{
		EndProne();
	}
	UnCrouch();
}

void ABattleCharacter::StartProne()
{
	bIsProne = true;

	GetCharacterMovement()->MaxWalkSpeed = ProneSpeed;
}

void ABattleCharacter::EndProne()
{
	bIsProne = false;
	if (bIsIronsight)
	{
		SetIronsight();
	}
	else
	{
		ReleaseIronsight();
	}
}

void ABattleCharacter::Inventory()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ToggleInventory();
	}
}

void ABattleCharacter::GetItem()
{
	AMasterItem* PickupItem = GetClosestItem();
	if (PickupItem && !PickupItem->IsPendingKill())
	{
		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			UItemSlotWidgetBase* Slot = PC->Inventory->GetSameIDSlot(PickupItem->ItemIndex);
			if (Slot)
			{
				Slot->AddItemCount(PickupItem->ItemCount);
				PickupItem->Destroy();
				ViewItemTooltip();
			}
			else
			{
				UItemSlotWidgetBase* NewSlot = PC->Inventory->GetEmptySlot();
				if (NewSlot)
				{
					NewSlot->SetItem(PickupItem->ItemIndex);
					PickupItem->Destroy();
					ViewItemTooltip();
				}
			}
		}
	}
}

void ABattleCharacter::Sprint()
{
	if (!bIsCrouched && !bIsIronsight && !bIsFire)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABattleCharacter::UnSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void ABattleCharacter::LookAround()
{
	if (!bIsIronsight)
	{
		bUseControllerRotationYaw = false;
		ControllerRotataion = GetController()->GetControlRotation();
		//UE_LOG(LogClass, Warning, TEXT("Rotation %s"), *(ControllerRotataion.ToString()));
	}
}

void ABattleCharacter::LookForward()
{
	bUseControllerRotationYaw = true;
	GetController()->SetControlRotation(ControllerRotataion);
}

void ABattleCharacter::StartFire()
{
	if (!bIsSprint && !bIsProning)
	{
		bIsFire = true;
		OnShot();
	}
}

void ABattleCharacter::StopFire()
{
	bIsFire = false;
}

void ABattleCharacter::OnShot()
{
	if (!bIsFire)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);

	FVector WorldLocation;
	FVector WorldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2,
		WorldLocation, WorldDirection);

	FVector StartTrace = CameraLocation;
	FVector EndTrace = CameraLocation + (WorldDirection * 80000.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ActorsToIgnore.Add(this);

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace,
		ObjectTypes, false,
		ActorsToIgnore, EDrawDebugTrace::None, OutHit,
		true, FLinearColor::Blue, FLinearColor::Black, 5.0f);

	FTransform MuzzleTransform = Weapon->GetSocketTransform(TEXT("MuzzleFlash"));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFireEffect, MuzzleTransform);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzleTransform.GetLocation(),
		MuzzleTransform.GetRotation().Rotator());

	if (Result)
	{
		StartTrace = MuzzleTransform.GetLocation();
		EndTrace = StartTrace + (OutHit.Location - StartTrace) * 2.0f;
		FHitResult OutHitResult;
		bool Hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace,
			ObjectTypes, false,
			ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHitResult,
			true, FLinearColor::Green, FLinearColor::Red, 5.0f);
		if (Hit)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OutHit.Location,
				OutHit.ImpactNormal.Rotation());

			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 0.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0),
				this, UBulletDamageType::StaticClass());

			//UGameplayStatics::ApplyRadialDamage(GetWorld(), 0.0f, OutHit.ImpactPoint, 300.0f, UBulletDamageType::StaticClass(), ActorsToIgnore, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), false);

			//UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 300.0f, 10.0f, OutHitResult.ImpactPoint,				20.0f, 300.0f, 1.0f, UBulletDamageType::StaticClass(), ActorsToIgnore, this, UGameplayStatics::GetPlayerController(GetWorld(), 0));

			UGameplayStatics::ApplyPointDamage(OutHitResult.GetActor(), 30.0f, EndTrace - StartTrace, OutHitResult, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UBulletDamageType::StaticClass());
		}
	}

	if (bIsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimeHandle, this, &ABattleCharacter::OnShot, 0.1f);
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(URifleCameraShake::StaticClass());
	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += 1.0f;
	CurrentRotation.Yaw += FMath::FRandRange(-0.05f, 0.05f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(CurrentRotation);
}

FRotator ABattleCharacter::GetAimoffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRot = AimDirLS.Rotation();
	return AimRot;
}

void ABattleCharacter::AddPickupItems(AMasterItem *Item)
{
	if (Item && !Item->IsPendingKill())
	{
		CanPickupItems.Add(Item);
	}

	ViewItemTooltip();
}

void ABattleCharacter::RemovePickupItems(AMasterItem *Item)
{
	if (Item)
	{
		CanPickupItems.Remove(Item);
	}
	ViewItemTooltip();
}

void ABattleCharacter::ViewItemTooltip()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}


	if (CanPickupItems.Num() == 0)
	{
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	AMasterItem* ClosestItem = GetClosestItem();
	if (ClosestItem)
	{
		//	UE_LOG(LogClass, Warning, TEXT("ItemName : %d"), ClosestItem->ItemIndex);
		PC->ItemTooltip->ItemName->SetText(FText::FromString(ClosestItem->ItemDataTable->GetItemData(ClosestItem->ItemIndex).ItemName));
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

AMasterItem* ABattleCharacter::GetClosestItem()
{
	AMasterItem* ClosestItem = nullptr;

	float Min = 9999999999.9f;
	for (auto Item : CanPickupItems)
	{
		float Distance = FVector::Dist(Item->GetActorLocation(), GetActorLocation());
		if (Min > Distance)
		{
			Min = Distance;
			ClosestItem = Item;
		}
	}

	return ClosestItem;
}
