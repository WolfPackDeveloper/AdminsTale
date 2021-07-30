// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ATCharacterBase.generated.h"

class UATResourceComponent;
class UATWeaponComponent;
class UAT_DamageTypeBase;

class UDamageType;

UENUM()
enum class EMovementBehaviour : uint8
{
	Run,
	Walk,
	Crouch
};

UCLASS()
class ADMINSTALE_API AATCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AATCharacterBase();

private:

	// Расчёт входящего урона - Damagetype, Resistance и т.д.
	float CountReceivedDamage(float DamageAmount, const UAT_DamageTypeBase* DamageType);
	// Расчёт входящего исцеления.
	float CountReceivedHealing(float HealAmount);

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Первая фаза процедуры умирания.
	UFUNCTION()
	void OnEndHealth();

	// Потом реализую - "прилипание" к ближайшему противнику (мягкий Aim) для небольшой доводки при ударах.
	// Будет работать как раз в состоянии боя - вот и пригодился делегат.
	//void TargetLock();

protected:

	// State

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "State")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	EMovementBehaviour MovementBehaviour = EMovementBehaviour::Run;
	
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

	// Вторая фаза процедуры умирания. Вызывается по AnimNotify в конце "смерти". Включение ragdoll и всякое такое.
	UFUNCTION()
	void OnDied();

public:	
	
	//FOnCombatting OnCombatting;

	bool IsMakingAction();

	float CalculateOuputDamage(float BaseDamage);

	// Action Bindings
	virtual void Jump() override;

	virtual void EquipWeapon(); // Пойдёт в компонент

	//virtual void Attack();

	//virtual void UsePower();

	virtual void Dash();

	//virtual void Aim();

	virtual void Action();

	UFUNCTION(BlueprintCallable)
	virtual EMovementBehaviour GetMovementBehaviour() const;

	virtual void SetMovementBehaviour(EMovementBehaviour Behaviour);
	
	//UFUNCTION(BlueprintCallable)
	//virtual bool IsInCombat() const;

	//virtual void SetCombatState(bool InCombat);

	UFUNCTION(BlueprintCallable)
	UATWeaponComponent* GetWeaponComponent() { return WeaponComponent; };

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
