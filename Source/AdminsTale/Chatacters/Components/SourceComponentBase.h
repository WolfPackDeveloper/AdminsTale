// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SourceComponentBase.generated.h"


UCLASS()
class ADMINSTALE_API USourceComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USourceComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//Changing source value
	UFUNCTION()
	virtual void SourceDown(float Value);
	UFUNCTION()
	virtual void SourceUp(float Value);

};
