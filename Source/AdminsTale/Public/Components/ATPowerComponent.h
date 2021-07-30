// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ATResourceComponent.h"
#include "ATPowerComponent.generated.h"

UCLASS()
class ADMINSTALE_API UATPowerComponent : public UATResourceComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UATPowerComponent();

private:

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual void Increase(float Value) override;

	virtual void Decrease(float Value) override;

};
