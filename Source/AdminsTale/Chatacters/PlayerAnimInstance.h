// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADMINSTALE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPlayerAnimInstance();

private:
	bool bIsSneaking = false;

protected:
	void SetSneakState(bool IsSneaking);


public:


};
