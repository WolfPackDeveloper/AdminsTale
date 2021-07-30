// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "AT_DamageTypeBase.generated.h"

UENUM()
enum class EDamageType : uint8
{
	Physic,
	Fire,
	Heal
};

UCLASS()
class ADMINSTALE_API UAT_DamageTypeBase : public UDamageType
{
	GENERATED_BODY()
	
public:

	UAT_DamageTypeBase();

private:


protected:

	EDamageType DamageType;

public:

	EDamageType GetType() const;
};
