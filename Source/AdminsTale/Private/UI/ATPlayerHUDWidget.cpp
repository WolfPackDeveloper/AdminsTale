// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ATPlayerHUDWidget.h"
#include "Characters/ATCharacterBase.h"
#include "Components/ATResourceComponent.h"

#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"

UATPlayerHUDWidget::UATPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

float UATPlayerHUDWidget::CountPercent(float CurrentValue, float MaxValue) const
{
	if (MaxValue != 0.f)
	{
		return CurrentValue / MaxValue;
	}
	else
	{
		return 0.f;
	}
}

void UATPlayerHUDWidget::OnHealthChanged(float CurrentValue, float MaxValue)
{
	HealthBar->SetPercent(CountPercent(CurrentValue, MaxValue));
}

void UATPlayerHUDWidget::OnPowerChanged(float CurrentValue, float MaxValue)
{
	PowerBar->SetPercent(CountPercent(CurrentValue, MaxValue));
}

//void UATPlayerHUDWidget::UpdateValues()
//{
//	const auto HealthComponent = Player->GetHealthComponent();
//	const auto PowerComponent = Player->GetPowerComponent();
//
//	// Update bars values.
//	OnHealthChanged(HealthComponent->GetCurrentAmount(), HealthComponent->GetMaxAmount());
//	OnPowerChanged(PowerComponent->GetCurrentAmount(), PowerComponent->GetMaxAmount());
//}

void UATPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<AATCharacterBase>(GetOwningPlayerPawn());

	// Binding delegates.
	if (IsValid(Player))
	{
		const auto HealthComponent = Player->GetHealthComponent();
		const auto PowerComponent = Player->GetPowerComponent();

		if (IsValid(HealthComponent))
		{
			HealthComponent->OnChange.AddDynamic(this, &UATPlayerHUDWidget::OnHealthChanged);
		}
		if (IsValid(PowerComponent))
		{
			PowerComponent->OnChange.AddDynamic(this, &UATPlayerHUDWidget::OnPowerChanged);
		}
	}
}
