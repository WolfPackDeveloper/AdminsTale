// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"


UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	DefaultMana = 100.f;
	Mana = DefaultMana;
}


// Called when the game starts
void UManaComponent::BeginPlay()
{
	Super::BeginPlay();
}

//Changing source value
void UManaComponent::SourceDown(float Value)
{
	Super::SourceDown(Value);
	
	if (Value <= 0)
	{
		return;
	}

	Mana = FMath::Clamp(Mana - Value, 0.f, DefaultMana);
}


void UManaComponent::SourceUp(float Value)
{
	Super::SourceUp(Value);
	
	if (Value <= 0)
	{
		return;
	}

	Mana = FMath::Clamp(Mana + Value, 0.f, DefaultMana);

}
