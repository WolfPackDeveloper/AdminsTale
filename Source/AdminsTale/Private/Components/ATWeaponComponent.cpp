// Fill out your copyright notice in the Description page of Project Settings.


#include "ATWeaponComponent.h"
#include "Actors/ATWeaponBase.h"
#include "Characters/ATCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h" // ����� GetMesh ��������. �� ��� ������� ���������� � ������������ ����� � ��� �� ������� �����.(

// Sets default values for this component's properties
UATWeaponComponent::UATWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UATWeaponComponent::SpawnWeapon()
{
	if (!IsValid(GetWorld()) || MeleeWeaponUnequippedSocketName == NAME_None)
	{
		return;
	}

	if (!IsValid(Character))
	{
		return;
	}

	MeleeWeapon = GetWorld()->SpawnActor<AATWeaponBase>(WeaponClass);

	if (!IsValid(MeleeWeapon))
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	MeleeWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, MeleeWeaponUnequippedSocketName);
	MeleeWeapon->SetOwner(Character);
}

void UATWeaponComponent::DrawWeapon()
{
	if (WeaponEquippedSocketName != NAME_None)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponEquippedSocketName);
	}

	WeaponToEquip = nullptr;
	Character->PlayAnimMontage(MontageEquipWeapon, MontageEquipPlayRate, WithWeaponSectionMontageName);
}

void UATWeaponComponent::SheatheWeapon(bool bReequip)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	
	if (EquippedWeapon == MeleeWeapon && MeleeWeaponUnequippedSocketName != NAME_None)
	{
		EquippedWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, MeleeWeaponUnequippedSocketName);
	}
	else if (EquippedWeapon == RangeWeapon && RangeWeaponUnequippedSocketName != NAME_None)
	{
		EquippedWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, RangeWeaponUnequippedSocketName);
	}

	if (!bReequip)
	{
		EquippedWeapon = nullptr;
		WeaponToEquip = nullptr;
		Character->PlayAnimMontage(MontageUnequipWeapon, MontageEquipPlayRate, WithoutWeaponSectionMontageName);
	}
}

void UATWeaponComponent::ReequipWeapon()
{
	SheatheWeapon(true);
	DrawWeapon();
}

// Called when the game starts
void UATWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AATCharacterBase>(GetOwner());

	SpawnWeapon();
}

void UATWeaponComponent::EquipWeapon(AATWeaponBase* Weapon)
{
	if (!IsValid(MontageEquipWeapon) || !IsValid(MontageUnequipWeapon))
	{
		return;
	}

	if (WithWeaponSectionMontageName == NAME_None || WithoutWeaponSectionMontageName == NAME_None)
	{
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	WeaponToEquip = Weapon;

	// �� ���� ��������� ��� ��������� - ������ �������� ������� � ���������� ���������� �������.
	// ���� � ����� ��� ������.
	if (!EquippedWeapon)
	{
		// �������������� �������� - ����� �� �������� � ��������� ����������� ���� (�� ���� ������������).
		if (EquippedWeapon == WeaponToEquip)
		{
			return;
		}
		else
		{
			Character->PlayAnimMontage(MontageEquipWeapon, MontageEquipPlayRate, WithoutWeaponSectionMontageName);
		}
	}
	// ���� ��� ���-�� �������. �� �������� ������������, � ��� ����� ����� (AttachWeapon).
	else
	{
		Character->PlayAnimMontage(MontageUnequipWeapon, MontageEquipPlayRate, WithWeaponSectionMontageName);
	}
}

void UATWeaponComponent::AttachWeapon()
{
	// ���� �� �������, �� �����������.
	if (!IsValid(EquippedWeapon) && IsValid(WeaponToEquip))
	{
		DrawWeapon();
	}
	// ���� ��� ���-�� �������, ��
	else
	{
		// ������� ������
		if (!IsValid(WeaponToEquip) && IsValid(EquippedWeapon) || EquippedWeapon == WeaponToEquip)
		{
			SheatheWeapon(false);
		}
		// ��� �� ���������������.
		else
		{
			ReequipWeapon();
		}
	}
}
void UATWeaponComponent::MeleeAttack()
{
	if (!IsValid(MeleeWeapon) || !IsValid(MontageMeleeAttack))
	{
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	// ���� ��� ����� �� ��������� ������� ��������.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	// ���� �� �������, ��� ������� �����-�� �����, ������ ������ ��������.
	if (EquippedWeapon != MeleeWeapon)
	{
		EquipWeapon(MeleeWeapon);
		// ���� ���, � �� ��� ������� ������� ������������ ���������� �����. �����, �����, ���-������ ��������.
		return;
	}
	// Attack
	// ������������� � ���������� ����� �� ������� �����������. ����� ����� ������ � ����� Character.
	//Character->TurnToAim(AimCharacter - Property? - What Class?);
	// ����� ����� �����, ��� ����� ������� �������, ������� ����� ������������, ����� ������ ������� �����������.
	// DefineComboAttackSection();
	FName AttackSection = MeleeAttackSectionMontageName;
	// PlayMontage
	if (AttackSection != NAME_None)
	{
		Character->PlayAnimMontage(MontageMeleeAttack, MontageAttackPlayRate, AttackSection);
	}
}

void UATWeaponComponent::RangeAttack()
{
	if (!IsValid(RangeWeapon) || !IsValid(MontageRangeAttack))
	{
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	// ���� ��� ����� �� ��������� ������� ��������.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	// ���� �� �������, ��� ������� �����-�� �����, ������ ������ ��������.
	if (EquippedWeapon != RangeWeapon)
	{
		EquipWeapon(RangeWeapon);
		// ���� ���, � �� ��� ������� ������� ������������ ���������� �����. �����, �����, ���-������ ��������.
		return;
	}
	// Attack
	// ����� ����� �����, ��� ����� ������� �������, ������� ����� ������������, ����� ������ ������� �����������.
	FName AttackSection = RangeAttackSectionMontageName;
	// PlayMontage
	if (AttackSection != NAME_None)
	{
		Character->PlayAnimMontage(MontageRangeAttack, MontageAttackPlayRate, AttackSection);
	}
}

bool UATWeaponComponent::IsWeaponEquipped() const
{
	return IsValid(EquippedWeapon);
}

AATWeaponBase* UATWeaponComponent::GetMeleeWeapon() const
{
	return MeleeWeapon;
}

// Called every frame
//void UATWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

