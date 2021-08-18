// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANSAttackTracer.h"

#include "Actors/ATWeaponBase.h"
#include "Characters/ATCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UANSAttackTracer::UANSAttackTracer()
{

}

void UANSAttackTracer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// ��� �����, ��� �� - MeshComp - ��� ������, �� ������� �������� ��������, �� ���� ��� ��������! �����!
	Character = Cast<AATCharacterBase>(MeshComp->GetOwner());
	if (IsValid(Character))
	{
		Weapon = Character->GetMeleeWeapon();

		ActorsToIgnore.Add(Character);
		ActorsToIgnore.Add(Weapon);

		EdgeBottomLastLocation = Weapon->GetWeaponMesh()->GetSocketLocation(WeaponEdgeBottom);
		BoxTraceHalfsize = Weapon->GetWeaponEdge();
	}
}

void UANSAttackTracer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!IsValid(Character))
	{
		return;
	}
	
	// Character->GetWorld() - ���������� �����. ����� ��������.
	FVector CurrentEdgeLocation = Weapon->GetWeaponMesh()->GetSocketLocation(WeaponEdgeBottom);
	FRotator CurrentEdgeRotation = Weapon->GetWeaponMesh()->GetSocketRotation(WeaponEdgeBottom);
	
	UKismetSystemLibrary::BoxTraceMulti(
		Character->GetWorld(),
		EdgeBottomLastLocation,
		CurrentEdgeLocation,
		BoxTraceHalfsize,
		CurrentEdgeRotation,
		ETraceTypeQuery::TraceTypeQuery4,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	for (int32 i = 0; i < HitResult.Num(); i++)
	{
		AActor* HitActor = HitResult[i].GetActor();
		
		if (!HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			// � ���� �������� ����� ��� ��������. ����� ����� ���������� ����� ���� ���� ��� ���������.
			Character->DealDamage(HitResult[i], Weapon);
		}
	}

	// � ��������� ����� �������� ��������� ������.
	EdgeBottomLastLocation = Weapon->GetWeaponMesh()->GetSocketLocation(WeaponEdgeBottom);
}

void UANSAttackTracer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	HitActors.Empty();
	//Character->ClearAim();
}
