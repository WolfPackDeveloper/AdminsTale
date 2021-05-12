// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEnemy.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

ACharacterEnemy::ACharacterEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default values

	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	bRunning = false;
	bSprinting = false;
	bSneaking = false;
	bCombatMode = false;
	bTargeted = false;

	FVector2D hbPivot = FVector2D(0.75, 0.75);
	FVector hbRelativeLocation = FVector(0.f, -20.f, 80.f);
	
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

void ACharacterEnemy::TargetPlayer()
{
}

void ACharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterEnemy::SetTargetedState(bool TargetState)
{
		bTargeted = TargetState;
		if (IsValid(HealthBar))
		{
			HealthBar->SetVisibility(TargetState);
		}
}
