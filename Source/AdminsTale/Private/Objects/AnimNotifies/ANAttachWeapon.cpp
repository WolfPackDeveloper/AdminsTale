// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANAttachWeapon.h"
#include "Characters/ATCharacterBase.h"
#include "Components/ATWeaponComponent.h"

#include "Components/SkeletalMeshComponent.h"

UANAttachWeapon::UANAttachWeapon()
{

}

void UANAttachWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		AATCharacterBase* Character = Cast<AATCharacterBase>(MeshComp->GetOwner());
		if (IsValid(Character))
		{
			Character->GetWeaponComponent()->AttachWeapon();
		}
	}
}