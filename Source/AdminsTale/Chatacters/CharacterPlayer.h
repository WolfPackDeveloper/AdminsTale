// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdminsTale/Chatacters/CharacterBase.h"
#include "Components/TimelineComponent.h"

#include "CharacterPlayer.generated.h"

// Вот это говно не работает без include! Почему?!
//class FOnTimelineVector;
class USpringArmComponent;
class UCameraComponent;
//class UCurveVector;
class USceneComponent;
//class UTimelineComponent;
//class UAnimMontage;

class ACharacterEnemy;
//class AWeapon;

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

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<AWeapon> WeaponClass;

	// Не уверен, надо ли именно так... В целом, решение - говно...
	TSubclassOf<ACharacterEnemy> EnemyBaseClass;
	
	//UPROPERTY()
	//ACharacterEnemy* TargetedEnemy = nullptr;

	float TargetRange = 1000.f;
	float TargetRadius = 300.f;
	bool bTargetMode = false;
	
	UPROPERTY(EditAnywhere, Category = "SpringArm")
	float saDefaultLength = 450.f;
	
	UPROPERTY(EditAnywhere, Category = "SpringArm")
	float saPitchSocketOffset = 60.f;
	
	UPROPERTY(EditAnywhere, Category = "SpringArm")
	float saYawSocketOffset = 30.f;
	
	FVector saDefaultRelativeLocation;
	FRotator saDefaultRelativeRotation;

	// Timeline testing ========================================================================
	
	////Float Track Signature to handle our update track event
	//FOnTimelineVector OnTimeVSAOffset;

	////Function which updates SpringArmOffset
	//UFUNCTION()
	//void UpdateSpringArmOffset(FVector Location);

	// =========================================================================================

	//Может быть засунуть это в функцию
	//FOnTimelineFloat UpdateTimelineProgress;
	
	//FTimeline CurveTimeline;

	// Для плавного оффсета палки при прицеливании.
	//void MoveSpringArmOffset(FVector ResultLocation);

	//TargetMode
	void GetClosestEnemy(TSubclassOf<ACharacterEnemy> EnemyClass, float Range);
	void SetPlayerRotationMode();
	void SimpleTargetLoking();
	void AdvancedTargetLocking(float DeltaTime);


protected:
		
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//USceneComponent* MeleeWeaponUnarmed = nullptr;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//USceneComponent* MeleeWeaponArmed = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//AWeapon* MeleeWeapon;

	UPROPERTY(BlueprintReadOnly)
	ACharacterEnemy* TargetedEnemy = nullptr;

	// Timeline testing ========================================================================

	////Camera offset moving.
	//UPROPERTY(EditAnywhere, Category = "Timeline")
	//UCurveVector* CurveSAVector;

	////TimelineComponent to animate door meshes
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UTimelineComponent* saTimelineComponent;

	// =========================================================================================

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Roll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName);

	// Пока таргетится ближайший противник, потом надо как-то переключатель придумать.
	UFUNCTION(BlueprintCallable)
	void TargetEnemy();

	UFUNCTION(BlueprintCallable)
	void StopTargetingEnemy();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	//UAbilitySystemComponent* AbilitySystemComponent = nullptr;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
