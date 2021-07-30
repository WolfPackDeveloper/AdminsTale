// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ATGameModeBase.generated.h"


UCLASS()
class ADMINSTALE_API AATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AATGameModeBase();

private:


protected:

	virtual void BeginPlay() override;

public:

};
