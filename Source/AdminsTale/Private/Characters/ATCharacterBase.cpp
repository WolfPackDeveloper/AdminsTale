// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterBase.h"
//#include "Actors/ATWeaponBase.h"
#include "Components/ATHealthComponent.h"
#include "Components/ATPowerComponent.h"
#include "Components/ATWeaponComponent.h"
#include "Objects/Damage/AT_DamageTypeBase.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
AATCharacterBase::AATCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UATHealthComponent>(TEXT("HealthComponent"));
	PowerComponent = CreateDefaultSubobject<UATPowerComponent>(TEXT("PowerComponent"));
	WeaponComponent = CreateDefaultSubobject<UATWeaponComponent>(TEXT("WeaponComponent"));

}

float AATCharacterBase::CountReceivedDamage(float DamageAmount, const UAT_DamageTypeBase* DamageType)
{
	return DamageAmount;
}

float AATCharacterBase::CountReceivedHealing(float HealAmount)
{
	return HealAmount;
}

void AATCharacterBase::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// PlayMontage OnTakeDamage

	//TODO: ��� ������ ����?
	const UAT_DamageTypeBase* IncomingDamageType = Cast<UAT_DamageTypeBase>(DamageType);

	if (IncomingDamageType->GetType() == EDamageType::Heal)
	{
		HealthComponent->Increase(CountReceivedHealing(Damage));
	}
	else
	{
		HealthComponent->Decrease(CountReceivedDamage(Damage, IncomingDamageType));
	}
}

void AATCharacterBase::OnEndHealth()
{
	if (!bIsDead)
	{
		bIsDead = true;

		// �������� ��������� ������  ��� ������������. ���������������, ��� ��������� ��������� �� ���� � ���������� ��������.
		int32 AnimCount = MontageOnDie->CompositeSections.Num() - 1;
		FName SectionName = MontageOnDie->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;

		// ����� �� �������, ���� ������� �� ���� �� ����� ������.
		if (IsValid(GetMesh()))
		{
			// ������ ��������� ��� ������������ ragdoll`a. ����, �������� ����� � ����� ��������.
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

		PlayAnimMontage(MontageOnDie, OnDiePlayRate, SectionName);
	}
}

// Called when the game starts or when spawned
void AATCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(PowerComponent)
	check(WeaponComponent);

	SetMovementBehaviour(MovementBehaviour);

	OnTakeAnyDamage.AddDynamic(this, &AATCharacterBase::OnTakeDamage);

	//SpawnWeapon();

	// �������� ���������.
	if (HealthComponent)
	{
		HealthComponent->OnEnd.AddDynamic(this, &AATCharacterBase::OnEndHealth);
	}
}

void AATCharacterBase::OnDied()
{
	//if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
	//{
	//	// ����� �� �������� �������� ��� ��������� ������. �� � ����� ����� ��������������.
	//	if (IsValid(MeleeWeapon))
	//	{
	//		// ����� ����� ���� � ���������
	//		MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//		MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
	//		MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	}

	//	// � ���� ��������� ���� ������ Ragdoll?
	//	GetMesh()->SetAllBodiesSimulatePhysics(true);
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	SetActorTickEnabled(false);
	//}
}

bool AATCharacterBase::IsMakingAction()
{
	if (GetMesh()->GetAnimInstance())
	{
		return GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	}

	return false;
}

float AATCharacterBase::CalculateOuputDamage(float BaseDamage)
{
	return BaseDamage;
}

void AATCharacterBase::Jump()
{
	Super::Jump();
}

void AATCharacterBase::EquipWeapon()
{
	//if (!IsValid(MontageEquipWeapon)) return;

	//float BlendTime = 0.2f;
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//if (AnimInstance->IsAnyMontagePlaying())
	//{
	//	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	//	if (CurrentMontage == MontageOnHit)
	//	{
	//		AnimInstance->Montage_Stop(BlendTime, CurrentMontage);
	//	}
	//	else
	//	{
	//		return;
	//	}
	//}

	//// �� ���� ����� ������ ���������� 0 ��� 1.
	//if (MontageEquipWeapon->IsValidSectionIndex(bEquip))
	//{
	//	PlayAnimMontage(MontageEquipWeapon, PlayRate, MontageEquipWeapon->GetSectionName(bEquip));
	//}
	//else
	//{
	//	//Debug
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error section index in EquipWeaponMontage.")));
	//}	
}

//void AATCharacterBase::Attack()
//{
//
//}

//void AATCharacterBase::UsePower()
//{
//
//}

void AATCharacterBase::Dash()
{

}

//void AATCharacterBase::Aim()
//{
//
//}

void AATCharacterBase::Action()
{

}

EMovementBehaviour AATCharacterBase::GetMovementBehaviour() const
{
	return MovementBehaviour;
}

void AATCharacterBase::SetMovementBehaviour(EMovementBehaviour Behaviour)
{
	switch (Behaviour)
	{
	case EMovementBehaviour::Crouch:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

		break;
	
	case EMovementBehaviour::Walk:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


		break;
	
	case EMovementBehaviour::Run:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

		break;
	
	default:
		
		break;
	}
}

//bool AATCharacterBase::IsInCombat() const
//{
//	return bIsInCombat;
//}
//
//void AATCharacterBase::SetCombatState(bool InCombat)
//{
//	bIsInCombat = InCombat;
//	//OnCombatting.Broadcast(bIsInCombat);
//}

// Called every frame
void AATCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AATCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

