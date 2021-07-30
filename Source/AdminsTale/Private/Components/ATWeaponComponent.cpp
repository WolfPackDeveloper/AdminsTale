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

	// ...
}

void UATWeaponComponent::SpawnWeapon()
{
	if (!IsValid(GetWorld()) || MeleeWeaponUnequippedSocketName == NAME_None)
	{
		return;
	}

	//ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!IsValid(Character))
	{
		return;
	}

	//const auto Weapon = GetWorld()->SpawnActor<AATWeaponBase>(WeaponClass);
	
	MeleeWeapon = GetWorld()->SpawnActor<AATWeaponBase>(WeaponClass);

	if (!IsValid(MeleeWeapon))
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	MeleeWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, MeleeWeaponUnequippedSocketName);
}

void UATWeaponComponent::ReequipWeapon(bool bReequip)
{
	if (!IsValid(MontageEquipWeapon) || !IsValid(MontageUnequipWeapon))
	{
		return;
	}

	if (WithWeaponSectionMontageName == NAME_None || WithoutWeaponSectionMontageName == NAME_None)
	{
		return;
	}

	// 1) ���� ������ �� ���������, �� �����������.
	// 2) ���� ������ ���������, � �� ����� ��������������� - ������� ������ � �����.
	// 3) ���� ������ ���������, � ����� ���������������.



	float InBlendOutTime = 0.1f;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	// �� ���������, ������ ��������. � �� ��� �������������, � � ����� ����� ����������� ������! ������!
	if (AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->Montage_Stop(InBlendOutTime);
	}

	// 1) PlayMontageUnequip WithWeapon;
	AnimInstance->Montage_JumpToSection(WithWeaponSectionMontageName, MontageUnequipWeapon);
	AnimInstance->Montage_Play(MontageUnequipWeapon, MontagePlayRate);
	// 2) Attach CurrentWeapon to Unarmed;
	// ������ - ��� �� AnimNotify �����...
	// 3) Attach WeaponToEquip to Armed;
	// ������ - ��� �� AnimNotify �����...
	// 4) Set WeaponToAttach to CurrentWeapon;
	// ��� ������ ������������� ����� ����� �������� ������...
	// 5) PlayMontageEQuip WithWeapon;
	AnimInstance->Montage_JumpToSection(WithWeaponSectionMontageName, MontageEquipWeapon);
	AnimInstance->Montage_Play(MontageEquipWeapon, MontagePlayRate);
}

// Called when the game starts
void UATWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AATCharacterBase>(GetOwner());

	SpawnWeapon();
}

void UATWeaponComponent::AttachWeapon(AATWeaponBase* Weapon, FName SocketName)
{
}

//void UATWeaponComponent::EquipRangeWeapon(bool Equip)
//{
//
//}
//
//void UATWeaponComponent::EquipMeleeWeapon(bool Equip)
//{
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
//}

void UATWeaponComponent::EquipWeapon(AATWeaponBase* Weapon, bool Equip)
{


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

	if (EquippedWeapon)
	{
		if (EquippedWeapon == RangeWeapon)
		{
			ReequipWeapon(MeleeWeapon);
		}
	}
	else
	{
		EquipWeapon(MeleeWeapon, true);
	}
	// Attack
	//PlayMontage();
}

void UATWeaponComponent::RangeAttack()
{

}

// Called every frame
//void UATWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

