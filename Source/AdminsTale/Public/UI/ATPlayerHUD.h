// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ATPlayerHUD.generated.h"

//class UUserWidget;
class UATPlayerHUDWidget;

/**
 * 
 */
UCLASS()
class ADMINSTALE_API AATPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	AATPlayerHUD();

private:

protected:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UATPlayerHUDWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;
};
