// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UHealthComponent;
class UManaComponent;

UCLASS()
class ADMINSTALE_API ACharacterBase : public ACharacter
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
	//Charactericstics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	UHealthComponent* Health = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	//float Mana = 100.f;
	UManaComponent* Mana = nullptr;

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
	float CrouchSpeed = 120.f;
	//State
	UPROPERTY(EditAnywhere, Category = "States")
	bool IsInAir = false;
	UPROPERTY(EditAnywhere, Category = "States")
	bool IsRunning = false;
	UPROPERTY(EditAnywhere, Category = "States")
	bool IsSprinting = false;
	UPROPERTY(EditAnywhere, Category = "States")
	bool IsSneaking = false;
	//Attack
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShotDistance = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ImpulseForce = 10.f;

	//FUNCTIONS
	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();
	//Movement
	void MoveForvard(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRate(float AxisValue);
	virtual void Jump() override;
	void Run();
	void Sprint();
	void Sneak();
	//Attacking
	void Attack();
	void Shoot();
	void StopShooting();


public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
