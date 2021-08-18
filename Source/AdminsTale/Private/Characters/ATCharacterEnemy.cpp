// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterEnemy.h"

#include "Components/WidgetComponent.h"

AATCharacterEnemy::AATCharacterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;


	const FVector2D hbPivot = FVector2D(0.75, 0.75);
	const FVector hbRelativeLocation = FVector(0.f, -20.f, 80.f);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetPivot(hbPivot);
	HealthBar->SetRelativeLocation(hbRelativeLocation);
	HealthBar->SetVisibility(false);

	BattleText = CreateDefaultSubobject<UWidgetComponent>(TEXT("BattleText"));
	BattleText->SetupAttachment(RootComponent);
	BattleText->SetWidgetSpace(EWidgetSpace::Screen);
	BattleText->SetPivot(hbPivot);
	BattleText->SetRelativeLocation(hbRelativeLocation);

}

void AATCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AATCharacterEnemy::SetCombatState(bool InCombat)
{
	AATCharacterBase::SetCombatState(InCombat);
	
	HealthBar->SetVisibility(bIsInCombat);
}

void AATCharacterEnemy::SetIsTarget(bool IsTarget)
{
	bIsTarget = IsTarget;
	
	// На будущее - отключение HealthBar`a по таймеру после удара.
	//HealthBar->SetVisibility(bIsInCombat);
}

void AATCharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

