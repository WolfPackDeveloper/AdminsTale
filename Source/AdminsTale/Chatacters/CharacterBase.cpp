// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"
#include "AdminsTale/Actors/Weapon.h"

#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h" //Debug string.
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GAS ������� �� ������ �����...
	//AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
		
	BaseTurnRate = 70.f;
	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	
	CurrentMovementStatus = EMovementStatus::Walk;
	
	bIsDead = false;
	Target = nullptr;

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	
}

// ��������� Ragdoll`�. ������ - � ���� �� ��� ������?
//void ACharacterBase::DyingActionDelayed()
//{
//	GetWorldTimerManager().ClearTimer(DelayTimer);
//
//	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
//	{
//		// ����� �� �������� �������� ��� ��������� ������. �� � ����� ����� ��������������.
//		if (IsValid(MeleeWeapon))
//		{
//			MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//			MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
//			MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		}
//
//		GetMesh()->SetAllBodiesSimulatePhysics(true);
//		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		SetActorTickEnabled(false);
//	}
//}

void ACharacterBase::SpawnWeapon()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	
	if (IsValid(MeleeWeapon))
	{
		//MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		// ���, � ���� �������� ��� ������?!
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, AttachmentRules);
		
		// ��� ��������
		MeleeWeapon->SetOwner(this);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	// �������� � ��������� �������.
	//MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	////�������� ����� ���������� ���� � ������! ������ ��� �������� � ��������� ����...
	//MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//MeleeWeapon->SetOwner(this);

	SpawnWeapon();

	// �������� ��������.
	if (Health)
	{
		Health->OnHealthEnded.AddDynamic(this, &ACharacterBase::OnHealthEnded);
	}

} 

void ACharacterBase::MakeAttack(float AttackDamageMultiplier, UAnimMontage* AnimMontage, float PlayRate, FName StartSection)
{
	if (!IsValid(MeleeWeapon)) return;

	// ��� ���� ������� - SetCapsuleRadius = 70.f.
	// ��� ��������� �� ���� ���� ����, ���� �������� ����� ������� ������ � ����.
	// TODO: ��������� ������� �������.
	//GetCapsuleComponent()->SetCapsuleRadius(70.f);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// ���� ��� ����� �� ��������� ������� ��������.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	DamageMultiplier = AttackDamageMultiplier;

	// �������� �� ������� �������.
	//if (!bCombatMode)
	//{
	//	bCombatMode = true;
	//	MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//}

	PlayAnimMontage(AnimMontage, PlayRate, StartSection);
}

void ACharacterBase::OnHit(bool bIsHitReaction)
{
	if (bIsHitReaction)
	{
		float BlendTime = 0.2f;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance->IsAnyMontagePlaying())
		{
			AnimInstance->StopAllMontages(BlendTime);
		}

		FName SectionName = MontageFastAttack->GetSectionName(0);

		PlayAnimMontage(MontageOnHit, OnHitPlayRate, SectionName);
	}

	if (!bCombatMode)
	{
		SetCombatMode(true);
	}
}

//void ACharacterBase::OnHit(bool bIsHitReaction, UAnimMontage* AnimMontage, float PlayRate, FName StartSection)
//{
//	//��� ����� ����� �� ������� ������.
//	//if (!bCombatMode)
//	//{
//	//	bCombatMode = true;
//	//	MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	//}
//
//	if (bIsHitReaction)
//	{
//		float BlendTime = 0.2f;
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//		if (AnimInstance->IsAnyMontagePlaying())
//		{
//			AnimInstance->StopAllMontages(BlendTime);
//		}
//
//		PlayAnimMontage(AnimMontage, PlayRate, StartSection);
//	}
//}

void ACharacterBase::EquipWeapon(bool bEquip, float PlayRate)
{
	if (!IsValid(MontageEquipWeapon)) return;

	float BlendTime = 0.2f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying())
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentMontage == MontageOnHit)
		{
			AnimInstance->Montage_Stop(BlendTime, CurrentMontage);
		}
		else
		{
			return;
		}
	}

	// �� ���� ����� ������ ���������� 0 ��� 1.
	if (MontageEquipWeapon->IsValidSectionIndex(bEquip))
	{
		PlayAnimMontage(MontageEquipWeapon, PlayRate, MontageEquipWeapon->GetSectionName(bEquip));
	}
	else
	{
		//Debug
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error section index in EquipWeaponMontage.")));
	}	
}

void ACharacterBase::SetCombatMode(bool bEnableCombatMode)
{
	// �������� ������ � ������ ������ ����� AnimNotify.
	bCombatMode = bEnableCombatMode;

	// ����� ����� ���� ��� �����-������ ������, ������ ������������.

	EquipWeapon(bCombatMode, EquipWeaponPlayRate);
}

void ACharacterBase::OnHealthEnded()
{

}

//void ACharacterBase::DyingAction(class UAnimMontage* AnimMontage, float InPlayRate, float DelayTime)
//{
//	if (!bIsDead)
//	{
//		bIsDead = true;
//
//		// �������� ��������� ������  ��� ������������.
//		int32 AnimCount = AnimMontage->CompositeSections.Num() - 1;
//		FName SectionName = AnimMontage->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;
//
//		// ����� �� �������, ���� ������� �� ���� �� ����� ������.
//		if (IsValid(GetMesh()))
//		{
//			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//		}
//
//		DetachFromControllerPendingDestroy();
//
//		// ����������� ��� ��������
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//		if (AnimInstance->IsAnyMontagePlaying())
//		{
//			//UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
//
//			//AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(Montage), Montage);
//			float BlendTime = 0.2f;
//			AnimInstance->StopAllMontages(BlendTime);
//		}
//
//		PlayAnimMontage(AnimMontage, InPlayRate, SectionName);
//
//		// ������ �� AnimNotify. ������ ������ ��� Delay.
//		//GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ACharacterBase::DyingActionDelayed, DelayTime, false);
//	}
//}

void ACharacterBase::OnDeathStart()
{
	if (!bIsDead)
	{
		bIsDead = true;

		// �������� ��������� ������  ��� ������������. ���������������, ��� ��������� ��������� �� ���� � ���������� ��������.
		int32 AnimCount = MontageOnDeath->CompositeSections.Num() - 1;
		FName SectionName = MontageOnDeath->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;

		// ����� �� �������, ���� ������� �� ���� �� ����� ������.
		if (IsValid(GetMesh()))
		{
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}

		DetachFromControllerPendingDestroy();

		// ����������� ��� ��������
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance->IsAnyMontagePlaying())
		{
			//UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();

			//AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(Montage), Montage);
			float BlendTime = 0.2f;
			AnimInstance->StopAllMontages(BlendTime);
		}

		PlayAnimMontage(MontageOnDeath, OnDeathPlayRate, SectionName);
	}
}

void ACharacterBase::OnDeathEnd()
{
	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
	{
		// ����� �� �������� �������� ��� ��������� ������. �� � ����� ����� ��������������.
		if (IsValid(MeleeWeapon))
		{
			MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
			MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// � ���� ��������� ���� ������ Ragdoll?
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
	}
}

void ACharacterBase::MoveForvard(float AxisValue)
{
	//������� �1 - ��� ���������� ��������� ��������� ���������������� ��.
	//AddMovementInput(GetActorForwardVector() * AxisValue);
	
	//������� �2 - �������� ����� �� �����, ��� ���������� ������� �������������� ��������.
	//��, �� ����, �������� ���� ������ ��������... �� ����� ����� ���������???!!!
	
	//������������� ���������, �� ����� �������� (�����, ������� � ������ �����)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	
	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//������ ��� �������� ���������� ����������� � �������, � � ������� � ��� ������???
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	//������������� ���������, �� ����� �������� (�����, ������� � ������ �����)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//������ ��� �������� ���������� ����������� � �������, � � ������� � ��� ������???
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::Jump()
{
	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		Super::Jump();
	}
}

void ACharacterBase::Run()
{
	if (CurrentMovementStatus == EMovementStatus::Run)
	{
		CurrentMovementStatus = EMovementStatus::Walk;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		CurrentMovementStatus = EMovementStatus::Run;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ACharacterBase::Sprint()
{
	CurrentMovementStatus = EMovementStatus::Sprint;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterBase::StopSprinting()
{
	CurrentMovementStatus = EMovementStatus::Run;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ACharacterBase::Sneak()
{
	if (CurrentMovementStatus == EMovementStatus::Sneack)
	{
		CurrentMovementStatus = EMovementStatus::Walk;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		CurrentMovementStatus = EMovementStatus::Sneack;
		GetCharacterMovement()->MaxWalkSpeed = SneakSpeed;
	}
}

void ACharacterBase::Walk()
{
	CurrentMovementStatus = EMovementStatus::Walk;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACharacterBase::MakeRoll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName)
{

}

void ACharacterBase::Roll_Implementation()
{
}

// Triggered by AnimNotify - AttachWeapon.
void ACharacterBase::AttachWeapon(bool bIsEquip)
{
	if (bIsEquip)
	{
		MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACharacterBase::SwitchCombatMode_Implementation()
{
	if (!IsValid(MeleeWeapon)) return;
	SetCombatMode(!bCombatMode);
}

void ACharacterBase::AttackFast_Implementation()
{
	if (!bCombatMode)
	{
		SwitchCombatMode();
		//SetCombatMode(true);
	}
	
	// ��� �������� ������ ������� MakeAttack.
	//DamageMultiplier = FastAttackDamageMultiplier;

	// ���� � ���������� ����� �����, �� ��� ����� ������ ������ ������ ��� ������������.
	
	// ...

	FName SectionName = MontageFastAttack->GetSectionName(0);

	MakeAttack(FastAttackDamageMultiplier, MontageFastAttack, FastAttackPlayRate, SectionName);
}

void ACharacterBase::AttackStrong_Implementation()
{
	if (!bCombatMode)
	{
		SwitchCombatMode();
		//SetCombatMode(true);
	}

	// ��� �������� ������ ������� MakeAttack.
	//DamageMultiplier = FastAttackDamageMultiplier;

	// ���� � ���������� ����� �����, �� ��� ����� ������ ������ ������ ��� ������������.

	// ...

	FName SectionName = MontageStrongAttack->GetSectionName(0);

	MakeAttack(StrongAttackDamageMultiplier, MontageStrongAttack, StrongAttackPlayRate, SectionName);
}

void ACharacterBase::Block_Implementation()
{

}

void ACharacterBase::TargetEnemy()
{

}

void ACharacterBase::StopTargetingEnemy()
{
}

void ACharacterBase::Action()
{

}

//UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

float ACharacterBase::CalculateDamageMultiplier()
{
	// ��� ������� ������, ����� ����� ��� �������.

	
	return DamageMultiplier;
}

EMovementStatus ACharacterBase::GetMovementStatus() const
{
	return CurrentMovementStatus;
}

void ACharacterBase::SetMovementStatus(EMovementStatus MovementStatus)
{
	switch (MovementStatus)
	{
	case EMovementStatus::Sneack:
		Sneak();
		break;
	case EMovementStatus::Walk:
		Walk();
		break;
	case EMovementStatus::Run:
		Run();
		break;
	case EMovementStatus::Sprint:
		Sprint();
		break;
	default:
		break;
	}
}

bool ACharacterBase::IsDead()
{
	return bIsDead;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
