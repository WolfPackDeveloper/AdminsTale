// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ATWeaponBase.h"
#include "ATWeaponMelee.generated.h"

UCLASS()
class ADMINSTALE_API AATWeaponMelee : public AATWeaponBase
{
	GENERATED_BODY()
	
public:
	AATWeaponMelee();

private:



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
