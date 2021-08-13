// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANEnableRagdoll.generated.h"

/**
 * 
 */
UCLASS()
class ADMINSTALE_API UANEnableRagdoll : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UANEnableRagdoll();

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
