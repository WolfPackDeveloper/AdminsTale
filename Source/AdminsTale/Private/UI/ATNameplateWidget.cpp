// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ATNameplateWidget.h"
#include "Characters/ATCharacterBase.h"
#include "Components/ATResourceComponent.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"

float UATNameplateWidget::CountPercent(float CurrentValue, float MaxValue) const
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

void UATNameplateWidget::OnHealthChanged(float CurrentValue, float MaxValue)
{
	if (IsVisible())
	{
		HealthBar->SetPercent(CountPercent(CurrentValue, MaxValue));
	}
}

void UATNameplateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<AATCharacterBase>(GetOwningPlayerPawn());

	// Binding delegates.
	if (IsValid(Character))
	{
		const auto HealthComponent = Character->GetHealthComponent();
		//const auto PowerComponent = Player->GetPowerComponent();

		if (IsValid(HealthComponent))
		{
			HealthComponent->OnChange.AddDynamic(this, &UATNameplateWidget::OnHealthChanged);
		}
		//if (IsValid(PowerComponent))
		//{
		//	PowerComponent->OnChange.AddDynamic(this, &UATPlayerHUDWidget::OnPowerChanged);
		//}

		//На потом. Когда придумаю с видимость этого виджета только для цели (со статусами и таймерами).
		//SetVisibility(ESlateVisibility::Hidden);
		SetVisibility(ESlateVisibility::Visible);
		IsVisible();
	}
}
