// Fill out your copyright notice in the Description page of Project Settings.


#include "ATWeaponBase.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AATWeaponBase::AATWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AATWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AATWeaponBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

