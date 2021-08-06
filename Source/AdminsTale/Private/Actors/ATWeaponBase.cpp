// Fill out your copyright notice in the Description page of Project Settings.


#include "ATWeaponBase.h"
#include "Damage/AT_DamageTypeBase.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AATWeaponBase::AATWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(WeaponMesh);

	EdgeTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EdgeTraceBox"));
	EdgeTraceBox->SetupAttachment(WeaponMesh);

}

// Called when the game starts or when spawned
void AATWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

USkeletalMeshComponent* AATWeaponBase::GetWeaponMesh() const
{
	return WeaponMesh;
}

EWeaponType AATWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

TSubclassOf<UAT_DamageTypeBase> AATWeaponBase::GetDamageType() const
{
	return DamageType;
}

float AATWeaponBase::GetBaseDamage() const
{
	return BaseDamage;
}

FVector AATWeaponBase::GetWeaponEdge()
{
	return EdgeTraceBox->GetScaledBoxExtent();
}

// Called every frame
//void AATWeaponBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

