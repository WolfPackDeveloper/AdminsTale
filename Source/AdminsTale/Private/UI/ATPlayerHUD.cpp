// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ATPlayerHUD.h"
#include "UI/ATPlayerHUDWidget.h"

//#include "Blueprint/UserWidget.h"

AATPlayerHUD::AATPlayerHUD()
{

}

void AATPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerHUDWidget = CreateWidget<UATPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);

	if (IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void AATPlayerHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
