// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_Base.h"

UDT_Base::UDT_Base()
{
	DamageMultiplier = 1.f;
}

float UDT_Base::ModifyDamage(float DamageAmount) const
{
	return DamageAmount * DamageMultiplier;
}
