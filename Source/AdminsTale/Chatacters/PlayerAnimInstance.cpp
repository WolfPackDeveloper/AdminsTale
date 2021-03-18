// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	bIsSneaking = false;
}

void UPlayerAnimInstance::SetSneakState(bool IsSneaking)
{
	bIsSneaking = IsSneaking;
}
