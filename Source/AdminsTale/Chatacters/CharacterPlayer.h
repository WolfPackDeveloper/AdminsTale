// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdminsTale/Chatacters/CharacterBase.h"
#include "CharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
//class USceletalMeshComponent;
class UAnimMontage;

class AWeapon;

UCLASS()
class ADMINSTALE_API ACharacterPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:

	ACharacterPlayer();
	
	//Переопределение метода интерфейса
	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	//virtual void Jump() override;

protected:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MeleeWeaponUnarmed = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MeleeWeaponArmed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* MeleeWeapon;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Roll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	//UAbilitySystemComponent* AbilitySystemComponent = nullptr;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
