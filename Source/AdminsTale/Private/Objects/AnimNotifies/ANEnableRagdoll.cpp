// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANEnableRagdoll.h"
#include "Characters/ATCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"

UANEnableRagdoll::UANEnableRagdoll()
{

}

void UANEnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AATCharacterBase* Character = Cast<AATCharacterBase>(MeshComp->GetOwner());
	if (IsValid(Character))
	{
		Character->OnDied();
	}
}
