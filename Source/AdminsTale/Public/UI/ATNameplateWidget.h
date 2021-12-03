// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ATNameplateWidget.generated.h"

class AATCharacterBase;

class UImage;
class UProgressBar;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class ADMINSTALE_API UATNameplateWidget : public UUserWidget
{
	GENERATED_BODY()

private:

protected:

	UPROPERTY()
	AATCharacterBase* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* NameplateBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* TargetMark = nullptr;

	UFUNCTION(BLueprintCallable, Category = "UI")
	float CountPercent(float CurrentValue, float MaxValue) const;

public:

	UFUNCTION(BLueprintCallable, Category = "UI")
	void OnHealthChanged(float CurrentValue, float MaxValue);

	virtual void NativeConstruct() override;

};
