// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AdminPlayerController.generated.h"

class ACharacterBase;
class UInputComponent;
class UAnimMontage;

UCLASS()
class ADMINSTALE_API AAdminPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	AAdminPlayerController();

private:

	UPROPERTY()
	ACharacterBase* PlayerCharacter = nullptr;

protected:

	// ==========
	// FUNCTIONS
	// ==========

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement
	void MoveForvard(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRate(float AxisValue);
	void Jump();
	void StopJumping();
	void Run();
	void Sprint();
	void StopSprinting();
	void Sneak();
	void Walk();
	void Roll();

	//Other
	void Action();

	//Combat Mode
	UFUNCTION()
	void SwitchCombatMode();

	//Attacking
	UFUNCTION()
	void AttackFast();

	UFUNCTION()
	void AttackStrong();

	// Пока таргетится ближайший противник, потом надо как-то переключатель придумать.
	UFUNCTION()
	void TargetEnemy();

	UFUNCTION()
	void StopTargetingEnemy();

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

public:

};
