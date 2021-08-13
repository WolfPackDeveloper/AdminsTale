// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"

#include "ATCoreTypes.h"
#include "AT_DamageTypeBase.generated.h"

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
