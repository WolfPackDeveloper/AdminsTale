// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/AT_DamageTypeBase.h"

UAT_DamageTypeBase::UAT_DamageTypeBase()
{

}

EDamageType UAT_DamageTypeBase::GetType() const
{
	return DamageType;
}
