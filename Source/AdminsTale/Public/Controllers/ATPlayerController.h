// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATPlayerController.generated.h"

class AATCharacterPlayer;

UCLASS()
class ADMINSTALE_API AATPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AATPlayerController();

private:

protected:

	// PROPERTIES
	UPROPERTY()
	AATCharacterPlayer* PlayerCharacter = nullptr;

	// FUNCTIONS
	// Bindings Movement
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRightRate(float AxisValue);

	// Bindings Actions with Gamepad
	void Jump();
	void StopJumping();
	void MeleeAttack();
	void RangeAttack();
	void Dash();
	//void Aim();
	// For testing
	void SheatheWeapon();
	void Action();
	// Keyboard only for now.
	void Run();
	void Crouch();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

public:

	virtual void Tick(float DeltaTime) override;
};
