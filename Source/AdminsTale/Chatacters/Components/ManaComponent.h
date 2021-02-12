// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdminsTale/Chatacters/Components/SourceComponentBase.h"
#include "ManaComponent.generated.h"

UCLASS()
class ADMINSTALE_API UManaComponent : public USourceComponentBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UManaComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float DefaultMana = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float Mana;
	
	//Changing source value
	virtual void SourceDown(float Value) override;
	virtual void SourceUp(float Value) override;

};
