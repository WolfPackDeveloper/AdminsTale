// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ATCoreTypes.h"
#include "ATCharacterBase.generated.h"

class AATWeaponBase;

class UATAimComponent;
class UATResourceComponent;
class UATWeaponComponent;

class UAT_DamageTypeBase;
class UDamageType;



UCLASS()
class ADMINSTALE_API AATCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AATCharacterBase();

private:

	// OnTakeDamage Handle

	void PlayOnHitMontage();
	void DefineDamageConsiquences(const UDamageType* DamageType, float Damage);

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// ������ ���� ��������� ��������.
	UFUNCTION()
	void OnEndHealth();

	// ����� �������� - "����������" � ���������� ���������� (������ Aim) ��� ��������� ������� ��� ������.
	// ����� �������� ��� ��� � ��������� ��� - ��� � ���������� �������.
	//void TargetLock();

protected:

	// State

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "State")
	bool bIsDead = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "State")
	bool bCanReactOnHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	EMovementBehaviour MovementBehaviour = EMovementBehaviour::Run;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	ECharacterType CharacterType = ECharacterType::Neutral;

	// Movement

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseTurnRate = 70.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RunSpeed = 350.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CrouchSpeed = 150.f;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UATAimComponent* AimComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UATResourceComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UATResourceComponent* PowerComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UATWeaponComponent* WeaponComponent = nullptr;

	// Animation

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageOnTakeDamage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float OnTakeDamagePlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageOnDie = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float OnDiePlayRate = 1.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ������ ��������� ����� - Damagetype, Resistance � �.�.
	virtual float CountReceivedDamage(float DamageAmount, const UAT_DamageTypeBase* DamageType);
	// ������ ��������� ���������.
	virtual float CountReceivedHealing(float HealAmount);
	// ������ ���������� �����.
	float CalculateOutputDamage(const AATWeaponBase* Weapon);

public:	
	
	bool IsMakingAction();

	// Action Bindings
	virtual void Jump() override;

	virtual void Dash();

	virtual void Action();

	virtual void MeleeAttack();

	virtual void RangeAttack();

	void DealDamage(const FHitResult& HitResult, const AATWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable)
	virtual EMovementBehaviour GetMovementBehaviour() const;

	UFUNCTION(BlueprintCallable)
	ECharacterType GetCharacterType() const;

	virtual void SetMovementBehaviour(EMovementBehaviour Behaviour);
	
	//UFUNCTION(BlueprintCallable)
	//virtual bool IsInCombat() const;

	//virtual void SetCombatState(bool InCombat);

	void ClearAim();

	// ������ ���� ��������� ��������. ���������� �� AnimNotify � ����� "������". ��������� ragdoll � ������ �����.
	UFUNCTION()
	void OnDied();

	//UFUNCTION(BlueprintCallable)
	//UATAimComponent* GetAimComponent() const { return AimComponent; };

	UFUNCTION(BlueprintCallable)
	UATWeaponComponent* GetWeaponComponent() const { return WeaponComponent; };

	UFUNCTION(BlueprintCallable)
	UATResourceComponent* GetHealthComponent() const { return HealthComponent; };

	UFUNCTION(BlueprintCallable)
	UATResourceComponent* GetPowerComponent() const { return PowerComponent; };

	// For AnimNotifyState - AttackTracer
	UFUNCTION(BlueprintCallable)
	AATWeaponBase* GetMeleeWeapon() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
