// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ATWeaponComponent.generated.h"

class AATCharacterBase;
class AATWeaponBase;

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADMINSTALE_API UATWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UATWeaponComponent();

private:

	//bool bWeaponEquipped = false;

	UPROPERTY()
	AATCharacterBase* Character = nullptr;

	UPROPERTY()
	AATWeaponBase* MeleeWeapon = nullptr;

	UPROPERTY()
	AATWeaponBase* RangeWeapon = nullptr;

	UPROPERTY()
	AATWeaponBase* EquippedWeapon = nullptr;

	UPROPERTY()
	AATWeaponBase* WeaponToEquip = nullptr;

	void SpawnWeapon();
	// Срабатывает от AnimNotify и определяет, что куда присобачивать.
	void ReequipWeapon(bool bReequip);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AATWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MeleeWeaponUnequippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName RangeWeaponUnequippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName WithWeaponSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName WithoutWeaponSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageEquipWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageUnequipWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageMeleeAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageRangeAttack = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float MontagePlayRate = 1.f;

public:	
	
	void AttachWeapon(AATWeaponBase* Weapon, FName SocketName);

	//void EquipRangeWeapon(bool Equip);

	//void EquipMeleeWeapon(bool Equip);

	void EquipWeapon(AATWeaponBase* Weapon, bool Equip);

	void MeleeAttack();

	void RangeAttack();

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
