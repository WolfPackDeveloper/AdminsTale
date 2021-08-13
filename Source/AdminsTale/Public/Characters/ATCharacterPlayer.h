// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ATCharacterBase.h"
#include "ATCharacterPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ADMINSTALE_API AATCharacterPlayer : public AATCharacterBase
{
	GENERATED_BODY()
	
public:
	AATCharacterPlayer();

private:

	FVector saDefaultRelativeLocation = FVector(0.f, 20.f, 70.f);
	FRotator saDefaultRelativeRotation = FRotator(0.f, 0.f, 0.f);
	float saDefaultLength = 500.f;
	float saPitchSocketOffset = 60.f;
	float saYawSocketOffset = 30.f;

	// Может... AimComponent?
	void SetDefaultRotationBehaviour();
	// Нахер вообще он нужен - поставим доводку, и хорош.
	void SetAimRotationBehaviour();
	void EnableAimRotation(bool EnableAimRotation);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	
	// Взять в цель ближайшего по ForwardVector`у противника.
	//virtual void TakeAim() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// FUNCTIONS
	
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRightRate(float AxisValue);

	virtual void MeleeAttack() override;
	virtual void RangeAttack() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
