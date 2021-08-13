// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "ATAimComponent.generated.h"

class AATCharacterBase;

class UCurveFloat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADMINSTALE_API UATAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UATAimComponent();

private:


protected:
	
	UPROPERTY()
	AATCharacterBase* Character = nullptr;

	// Aiming handle

	FTimeline TurnTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	UCurveFloat* TurnCurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float AimRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float AimRange = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim", meta = (ClampMin = "1.0", ClampMax = "90.0"))
	float AimAngle = 60.f;

	UPROPERTY(BlueprintReadWrite)
	AActor* AimedActor = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	// Взять в цель ближайшего по направлению враждебного персонажа (В общем-то это для Player`а больше)...
	UFUNCTION()
	virtual void TakeAim();

	UFUNCTION(BlueprintCallable)
	AActor* GetAim() const;

	UFUNCTION(BlueprintCallable)
	void ClearAim();

	// Доводка к цели перед ударом. Для всех.
	UFUNCTION(BlueprintCallable)
	void TurnToAim(float CurveValue);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
