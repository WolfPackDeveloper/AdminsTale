// Fill out your copyright notice in the Description page of Project Settings.


#include "ATWeaponComponent.h"
#include "Actors/ATWeaponBase.h"
#include "Characters/ATCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h" // Иначе GetMesh ругается. Но эта история начинается с подключением одних и тех же хэдеров везде.(

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

	// Не знаю насколько это адекватно - логика принятия решения о вооружении конкретным оружием.
	// Если в руках нет оружия.
	if (!EquippedWeapon)
	{
		// Мразматическая ситуация - когда не снаряжен и пытаешься вооружиться нулём (то бишь разоружиться).
		if (EquippedWeapon == WeaponToEquip)
		{
			return;
		}
		else
		{
			Character->PlayAnimMontage(MontageEquipWeapon, MontageEquipPlayRate, WithoutWeaponSectionMontageName);
		}
	}
	// Если уже чем-то вооружён. То начинаем разоружяться, а там видно будет (AttachWeapon).
	else
	{
		Character->PlayAnimMontage(MontageUnequipWeapon, MontageEquipPlayRate, WithWeaponSectionMontageName);
	}
}

void UATWeaponComponent::AttachWeapon()
{
	// Если не вооружён, то вооружаемся.
	if (!IsValid(EquippedWeapon) && IsValid(WeaponToEquip))
	{
		DrawWeapon();
	}
	// Если уже чем-то вооружён, то
	else
	{
		// убираем оружие
		if (!IsValid(WeaponToEquip) && IsValid(EquippedWeapon) || EquippedWeapon == WeaponToEquip)
		{
			SheatheWeapon(false);
		}
		// или же перевооружаемся.
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
	// Пока что атака не прерывает никакие анимации.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	// Если не вооружён, или вооружён какой-то хернёй, срочно меняем ситуацию.
	if (EquippedWeapon != MeleeWeapon)
	{
		EquipWeapon(MeleeWeapon);
		// Пока так, а то вся красота затрётся моментальным исполнение атаки. Потом, может, что-нибудь придумаю.
		return;
	}
	// Attack
	// Прицеливаемся к ближайшему врагу по вектору направления. Будет перед атакой в самом Character.
	//Character->TurnToAim(AimCharacter - Property? - What Class?);
	// Когда будут комбы, тут нужно сделать функцию, которая будет рассчитывать, какую секцию монтажа проигрывать.
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
	// Пока что атака не прерывает никакие анимации.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	// Если не вооружён, или вооружён какой-то хернёй, срочно меняем ситуацию.
	if (EquippedWeapon != RangeWeapon)
	{
		EquipWeapon(RangeWeapon);
		// Пока так, а то вся красота затрётся моментальным исполнение атаки. Потом, может, что-нибудь придумаю.
		return;
	}
	// Attack
	// Когда будут комбы, тут нужно сделать функцию, которая будет рассчитывать, какую секцию монтажа проигрывать.
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

