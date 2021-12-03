// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ATWidgetComponent.h"
#include "Characters/ATCharacterBase.h"

#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UATWidgetComponent::UATWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Character = Character = Cast<AATCharacterBase>(GetOwner());

	if (!IsValid(Character))
	{
		return;
	}

	const FVector2D NameplatePivot = FVector2D(0.75, 0.75);
	const FVector NameplateRelativeLocation = FVector(0.f, -20.f, 80.f);

	// Nameplate

	Nameplate = CreateDefaultSubobject<UWidgetComponent>(TEXT("Nameplate"));
	Nameplate->SetupAttachment(Character->GetRootComponent());
	Nameplate->SetWidgetSpace(EWidgetSpace::Screen);
	Nameplate->SetPivot(NameplatePivot);
	Nameplate->SetRelativeLocation(NameplateRelativeLocation);
	Nameplate->SetVisibility(true);
	
	//CombatText

	//BattleText = CreateDefaultSubobject<UWidgetComponent>(TEXT("BattleText"));
	//BattleText->SetupAttachment(RootComponent);
	//BattleText->SetWidgetSpace(EWidgetSpace::Screen);
	//BattleText->SetPivot(hbPivot);
	//BattleText->SetRelativeLocation(hbRelativeLocation);


}


// Called when the game starts
void UATWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
//void UATWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

