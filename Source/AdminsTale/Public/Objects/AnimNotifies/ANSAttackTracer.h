// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANSAttackTracer.generated.h"

class AActor;

class AATCharacterBase;
class AATWeaponBase;

struct FHitResult;

UCLASS()
class ADMINSTALE_API UANSAttackTracer : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UANSAttackTracer();

private:

	UPROPERTY()
	TArray<FHitResult>HitResult; //Save the hit result

	UPROPERTY()
	TArray<AActor*>HitActors; // Save the hit Actor

	UPROPERTY()
	TArray<AActor*>ActorsToIgnore; // Ray ignores target

	FName WeaponEdgeBottom;

	//FName WeaponEdgeTop;

	FVector EdgeBottomLastLocation; // Slot 1 location

	//FVector EdgeTopLastLocation; // Slot 2 location

	FVector BoxTraceHalfsize;

	UPROPERTY()
	AATCharacterBase* Character; //Current character//

	UPROPERTY()
	AATWeaponBase* Weapon; // Characters weapon

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
