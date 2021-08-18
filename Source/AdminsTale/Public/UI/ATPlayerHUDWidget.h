// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ATPlayerHUDWidget.generated.h"

class AATCharacterBase;

class UProgressBar;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class ADMINSTALE_API UATPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UATPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

private:


protected:
	
	UPROPERTY()
	AATCharacterBase* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ResourceBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* PowerBar = nullptr;

	UFUNCTION(BLueprintCallable, Category = "UI")
	float CountPercent(float CurrentValue, float MaxValue) const;

public:

	UFUNCTION(BLueprintCallable, Category = "UI")
	void OnHealthChanged(float CurrentValue, float MaxValue);

	UFUNCTION(BLueprintCallable, Category = "UI")
	void OnPowerChanged(float CurrentValue, float MaxValue);

	virtual void NativeConstruct() override;
};
