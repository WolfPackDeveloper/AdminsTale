// Fill out your copyright notice in the Description page of Project Settings.


#include "ATResourceComponent.h"

// Sets default values for this component's properties
UATResourceComponent::UATResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UATResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxAmount > 0);

	//CurrentAmount = MaxAmount;
	Increase(MaxAmount);
}

void UATResourceComponent::Decrease(float Value)
{
	if (Value > 0.f)
	{
		CurrentAmount = FMath::Clamp(CurrentAmount - Value, 0.f, MaxAmount);
	}

	OnChange.Broadcast(CurrentAmount, MaxAmount);
}

void UATResourceComponent::Increase(float Value)
{
	if (Value > 0.f)
	{
		CurrentAmount = FMath::Clamp(CurrentAmount + Value, 0.f, MaxAmount);
	}

	if (CurrentAmount == MaxAmount)
	{
		bIsRecover = false;
	}

	OnChange.Broadcast(CurrentAmount, MaxAmount);
}

float UATResourceComponent::GetCurrentAmount()
{
	return CurrentAmount;
}

float UATResourceComponent::GetMaxAmount() const
{
	return MaxAmount;
}

void UATResourceComponent::SetMaxAmount(float Value)
{
	if (Value >= 0.f)
	{
		MaxAmount = Value;
	}
}

bool UATResourceComponent::IsRecover() const
{
	return bIsRecover;
}

void UATResourceComponent::EnableRecovery(bool Enable)
{
	bIsRecover = Enable;
}

float UATResourceComponent::GetRecoveryAmount() const
{
	return RecoveryAmount;
}

void UATResourceComponent::SetRecoveryAmount(float Value)
{
	if (Value >= 0.f)
	{
		RecoveryAmount = Value;
	}
}

float UATResourceComponent::GetRecoveryTickFrequency() const
{
	return RecoveryTickFrequency;
}

void UATResourceComponent::SetRecoveryTickFrequency(float Value)
{
	if (Value >= 0.f)
	{
		RecoveryTickFrequency = Value;
	}
}
