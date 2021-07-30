// Fill out your copyright notice in the Description page of Project Settings.


#include "ATPowerComponent.h"

UATPowerComponent::UATPowerComponent()
{

}

void UATPowerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UATPowerComponent::Decrease(float Value)
{
	UATResourceComponent::Decrease(Value);
}

void UATPowerComponent::Increase(float Value)
{
	UATResourceComponent::Increase(Value);
}
