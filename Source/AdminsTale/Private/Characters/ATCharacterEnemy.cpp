// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterEnemy.h"

#include "Components/WidgetComponent.h"

AATCharacterEnemy::AATCharacterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AATCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AATCharacterEnemy::SetCombatState(bool InCombat)
{
	AATCharacterBase::SetCombatState(InCombat);
	
	//WidgetComponent->Nameplate->SetVisibility(bIsInCombat);
}

void AATCharacterEnemy::SetIsTarget(bool IsTarget)
{
	bIsTarget = IsTarget;
	
	// На будущее - отключение HealthBar`a по таймеру после удара.
	//WidgetComponent->Nameplate->SetVisibility(bIsInCombat);
}

void AATCharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

