// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ATCharacterBase.generated.h"

class AATWeaponBase;
class UATResourceComponent;
class UATWeaponComponent;
class UAT_DamageTypeBase;

class UDamageType;

// Дерьмо для определения френдли фаера и буцкания бочек...
UENUM()
enum class ECharacterType : uint8
{
	Neutral,
	Player,
	Enemy,
	Ally
};

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

	// OnTakeDamage Handle

	void PlayOnHitMontage();
	void DefineDamageConsiquences(const UDamageType* DamageType, float Damage);

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

	// Other
	UPROPERTY(BlueprintReadWrite)
	AActor* Aim = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Вторая фаза процедуры умирания. Вызывается по AnimNotify в конце "смерти". Включение ragdoll и всякое такое.
	UFUNCTION()
	void OnDied();

	// Расчёт входящего урона - Damagetype, Resistance и т.д.
	virtual float CountReceivedDamage(float DamageAmount, const UAT_DamageTypeBase* DamageType);
	// Расчёт входящего исцеления.
	virtual float CountReceivedHealing(float HealAmount);
	// Расчёт исходящего урона.
	float CalculateOutputDamage(const AATWeaponBase* Weapon);

	// Aiming handle

	// Взять в цель ближайшего по направлению враждебного персонажа (В общем-то это для Player`а больше)...
	UFUNCTION(BlueprintCallable)
	virtual void TakeAim();

	// Доводка к цели перед ударом. Для всех.
	UFUNCTION(BlueprintCallable)
	void TurnToAim(); // Наверное, правильнее будет, всё таки, Property.

public:	
	
	bool IsMakingAction();

	// Action Bindings
	virtual void Jump() override;

	virtual void Dash();

	virtual void Action();

	void DealDamage(const FHitResult& HitResult, const AATWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable)
	virtual EMovementBehaviour GetMovementBehaviour() const;

	UFUNCTION(BlueprintCallable)
	ECharacterType GetCharacterType() const;

	virtual void SetMovementBehaviour(EMovementBehaviour Behaviour);
	
	//UFUNCTION(BlueprintCallable)
	//virtual bool IsInCombat() const;

	//virtual void SetCombatState(bool InCombat);

	UFUNCTION(BlueprintCallable)
	UATWeaponComponent* GetWeaponComponent() const { return WeaponComponent; };

	// For AnimNotifyState - AttackTracer
	UFUNCTION(BlueprintCallable)
	AATWeaponBase* GetMeleeWeapon() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetAim() const;

	UFUNCTION(BlueprintCallable)
	void SetAim(AActor* NewAim);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
