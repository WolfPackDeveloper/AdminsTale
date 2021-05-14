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

UCLASS()
class ADMINSTALE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//PROPERTIES
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
	AWeapon* MeleeWeapon;

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
	
	//State
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool bRunning = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool bSprinting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool bSneaking = false;

	//Battle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bInBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bCombatMode = false;

	// Damage Dealing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1;
	
	// ==========
	// FUNCTIONS
	// ==========

	//Movement
	virtual void MoveForvard(float AxisValue);
	
	virtual void MoveRight(float AxisValue);
	
	virtual void LookUpRate(float AxisValue);
	
	virtual void TurnRate(float AxisValue);
	
	virtual void Jump() override;
	
	void Run();
	
	void Sprint();
	
	void Sneak();

	//Combat Mode
	UFUNCTION()
	void EnableCombatMode(bool bEnable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatMode();
	
	void SetCombatMode_Implementation();

	//Attacking
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackFast();

	void AttackFast_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackStrong();

	void AttackStrong_Implementation();

	// Health
	// Delegate
	void OnHealthEnded();
	//Delegate content
	void DyingAction();

	//Other
	void Action();

public:	

	//Переопределение метода интерфейса
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Коэффициент используется в расчёте наносимого урона в оружии.
	float CalculateDamageMultiplier();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
