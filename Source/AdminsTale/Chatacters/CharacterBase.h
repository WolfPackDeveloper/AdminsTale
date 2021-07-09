// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UHealthComponent;
class USceneComponent;
class AWeapon;

class UAnimMontage;

UENUM()
enum class EMovementStatus : uint8
{
	Sneack,
	Walk,
	Run,
	Sprint
};

UCLASS()
class ADMINSTALE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

private:

	//Delay();
	FTimerHandle DelayTimer;

	// OnHealthEnded - Delegate delayed content
	void DyingActionDelayed();

protected:
	
	float TargetRange = 1000.f;

	float TargetRadius = 300.f;

	bool bTargetMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool bReactOnHit = true;
	
	// Movement state
	EMovementStatus CurrentMovementStatus = EMovementStatus::Walk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;

	//Charactericstics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	UHealthComponent* Health = nullptr;
	
	//Weapon (even empty)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MeleeWeaponUnarmed = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MeleeWeaponArmed = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* MeleeWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageEquipWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageFastAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageStrongAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageOnHit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageOnDeath = nullptr;

	//Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseTurnRate = 70.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 420.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SneakSpeed = 120.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Animation Config")
	float EquipWeaponPlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Animation Config")
	float FastAttackPlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Animation Config")
	float StrongAttackPlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Animation Config")
	float OnHitPlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Animation Config")
	float OnDeathPlayRate = 1.f;

	// Combat
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	//bool bInCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCombatMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

	// Damage Dealing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FastAttackDamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrongAttackDamageMultiplier = 1.5f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Для всякого AI и BT
	UPROPERTY(BlueprintReadWrite)
	AActor* Target = nullptr;

	// Combat
	UFUNCTION(BlueprintCallable)
	virtual void MakeAttack(float AttackDamageMultiplier, UAnimMontage* AnimMontage, float PlayRate, FName StartSection);

	UFUNCTION(BlueprintCallable)
	//virtual void OnHit(bool bIsHitReaction, UAnimMontage* AnimMontage, float PlayRate, FName StartSection);
	virtual void OnHit(bool bIsHitReaction, float PlayRate);

	UFUNCTION(BlueprintCallable)
	virtual void OnDeathEnd();

	//UFUNCTION(BlueprintCallable)
	//void ChangeCombatStance(UAnimMontage* AnimMontage, float PlayRate, FName StartSection);
	
	UFUNCTION()
	void EquipWeapon(bool bEquip, float PlayRate);

	UFUNCTION(BlueprintCallable)
	void SetCombatMode(bool bEnableCombatMode);

	// Health
	// Delegate
	void OnHealthEnded();
	//Delegate content
	UFUNCTION(BlueprintCallable)
	void DyingAction(UAnimMontage* AnimMontage, float InPlayRate, float DelayTime);

public:	

	//Переопределение метода интерфейса
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Movement
	virtual void MoveForvard(float AxisValue);

	virtual void MoveRight(float AxisValue);

	virtual void LookUpRate(float AxisValue);

	virtual void TurnRate(float AxisValue);

	virtual void Jump() override;

	virtual void Run();

	virtual void Sprint();

	virtual void StopSprinting();

	virtual void Sneak();

	virtual void Walk();

	UFUNCTION(BlueprintCallable)
	virtual void MakeRoll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Roll();
	virtual void Roll_Implementation();

	// Other
	virtual void Action();

	// Combat
	// Коэффициент используется в расчёте наносимого урона в оружии.
	float CalculateDamageMultiplier();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//void SetCombatMode();
	//virtual void SetCombatMode_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual void AttachWeapon(bool bIsEquip);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchCombatMode();
	virtual void SwitchCombatMode_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackFast();
	virtual void AttackFast_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackStrong();
	virtual void AttackStrong_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Block();
	virtual void Block_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual void TargetEnemy();

	UFUNCTION(BlueprintCallable)
	virtual void StopTargetingEnemy();

	// Movement
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMovementStatus GetMovementStatus() const;

	UFUNCTION(BlueprintCallable)
	void SetMovementStatus(EMovementStatus MovementStatus);

	// Other
	UFUNCTION(BlueprintCallable)
	bool IsDead();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
