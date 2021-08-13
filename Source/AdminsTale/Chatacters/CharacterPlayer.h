// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdminsTale/Chatacters/CharacterBase.h"
//#include "Components/TimelineComponent.h"

#include "CharacterPlayer.generated.h"

//struct FOnTimelineVector;
class USpringArmComponent;
class UCameraComponent;
//class UCurveVector;
class USceneComponent;
//class UTimelineComponent;
//class UAnimMontage;

class ACharacterEnemy;

UCLASS()
class ADMINSTALE_API ACharacterPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:

	ACharacterPlayer();

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

private:

	// Не уверен, надо ли именно так... В целом, решение - говно...
	TSubclassOf<ACharacterEnemy> EnemyBaseClass;
	
	//float TargetRange = 1000.f;
	//float TargetRadius = 300.f;
	//bool bTargetMode = false;
	
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

	// Aiming
	float AimRange = 1000.f;
	float AimRadius = 300.f;
	bool bAimMode = false;

	UPROPERTY(BlueprintReadOnly)
	ACharacterEnemy* AimedEnemy = nullptr;

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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	//UAbilitySystemComponent* AbilitySystemComponent = nullptr;

public:
	
	//Debug. Может поможет с камерой, как в Darksiders. Получение вектора направления по-другому.
	UFUNCTION(BlueprintCallable)
	void TraceForwardAndVelocity();

	//UFUNCTION(BlueprintCallable)
	virtual void MakeRoll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName) override;

	// Пока таргетится ближайший противник, потом надо как-то переключатель придумать.
	//UFUNCTION(BlueprintCallable)
	virtual void TargetEnemy() override;

	//UFUNCTION(BlueprintCallable)
	virtual void StopTargetingEnemy() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
