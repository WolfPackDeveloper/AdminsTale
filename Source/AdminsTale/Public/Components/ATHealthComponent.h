// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ATResourceComponent.h"
#include "ATHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADMINSTALE_API UATHealthComponent : public UATResourceComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UATHealthComponent();

private:

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void Decrease(float Value) override;

	virtual void Increase(float Value) override;
	
	// TODO. Будет реализовано в классе баффов и дебаффов. GameplayAbilitySystem.
	//void DamageOverTime(float DoTTick, float Damage);
	//void HealOverTime(float HoTTick, float Heal);
};
