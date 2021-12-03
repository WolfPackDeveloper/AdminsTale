// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ATCharacterBase.h"
#include "ATCharacterEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class ADMINSTALE_API AATCharacterEnemy : public AATCharacterBase
{
	GENERATED_BODY()
	
public:

	AATCharacterEnemy();

private:



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetCombatState(bool InCombat) override;

public:

	// При взятии в цель включается виджет здоровья и маркер... А как он выключается?
	virtual void SetIsTarget(bool IsTarget);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
