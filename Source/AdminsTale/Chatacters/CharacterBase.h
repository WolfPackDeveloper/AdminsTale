// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UHealthComponent;
class UAbilitySystemComponent;

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
	bool IsRunning = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool IsSprinting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool IsSneaking = false;

	//Battle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool IsInBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool IsBattleModeOn = false;
	
	// ==========
	// FUNCTIONS
	// ==========

	//Movement
	void MoveForvard(float AxisValue);
	
	void MoveRight(float AxisValue);
	
	void LookUpRate(float AxisValue);
	
	void TurnRate(float AxisValue);
	
	virtual void Jump() override;
	
	void Run();
	
	void Sprint();
	
	void Sneak();

	//Battle

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableBattleMode();

	void EnableBattleMode_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableBattleMode();

	void DisableBattleMode_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBattleMode();
	
	void SetBattleMode_Implementation();

	//Attacking

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackFast();

	void AttackFast_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackStrong();

	void AttackStrong_Implementation();

	void Action();

public:	
	//Переопределение метода интерфейса
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//UFUNCTION(BlueprintPure)
	//bool GetIsRunning();

	//UFUNCTION(BlueprintPure)
	//bool GetIsSprinting();

	//UFUNCTION(BlueprintPure)
	//bool GetIsSneaking();
		
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
