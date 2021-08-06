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
	
	// ¬нутренн€€ логика AttachWeapon.
	void DrawWeapon();
	void SheatheWeapon(bool bReequip);
	void ReequipWeapon();

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

	// Animation

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName WithWeaponSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName WithoutWeaponSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName MeleeAttackSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName RangeAttackSectionMontageName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageEquipWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageUnequipWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageMeleeAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageRangeAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float MontageEquipPlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float MontageAttackPlayRate = 1.f;

public:	
	
	/**
	* EquipWeapon - triggered, when player initiates an attack, or press buttom of equipping weapon.
	*
	*/
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AATWeaponBase* Weapon);
	//void EquipWeapon(AATWeaponBase* Weapon, bool Equip);

	/**
	* AttachWeapon - triggered by AnimNotify while equipping or unequipping weapon.
	*
	*/
	UFUNCTION(BlueprintCallable)
	void AttachWeapon();

	void MeleeAttack();

	void RangeAttack();

	/**
	* IsWeaponEquipped - bool for animations.
	*
	*/
	UFUNCTION(BlueprintCallable)
	bool IsWeaponEquipped() const;

	/**
	* GetMeleeWeapon - returns pointer on melee weapon.
	*
	*/
	UFUNCTION(BlueprintCallable)
	AATWeaponBase* GetMeleeWeapon() const;

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
