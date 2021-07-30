// Fill out your copyright notice in the Description page of Project Settings.


#include "ATHealthComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UATHealthComponent::UATHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UATHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UATHealthComponent::Decrease(float Value)
{
	UATResourceComponent::Decrease(Value);

	if (FMath::IsNearlyZero(CurrentAmount))
	{
		OnEnd.Broadcast();
	}
}

void UATHealthComponent::Increase(float Value)
{
	UATResourceComponent::Increase(Value);
}
