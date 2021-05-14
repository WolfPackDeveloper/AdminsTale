// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DT_Base.generated.h"


UCLASS()
class ADMINSTALE_API UDT_Base : public UDamageType
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DamageMultiplier = 1.f;

public:

	UDT_Base();

	float ModifyDamage(float DamageAmount) const;

};
