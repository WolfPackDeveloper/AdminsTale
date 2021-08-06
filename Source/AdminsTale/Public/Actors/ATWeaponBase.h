// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATWeaponBase.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;

class UAT_DamageTypeBase;

UENUM()
enum class EWeaponType : uint8
{
	Axe,
	Hammer,
	Spear,
	Staff,
	Sword
};

UCLASS()
class ADMINSTALE_API AATWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATWeaponBase();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	// Чтобы не рассчитывать в AnimNotifyState размер коробки для трейса.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* EdgeTraceBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon stats")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon stats")
	TSubclassOf<UAT_DamageTypeBase> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon stats", meta = (ClampMin = "1.0", ClampMax = "1000.0"))
	float BaseDamage = 1.f;

	// Dev Data

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dev Data")
	FName EdgeBottomSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dev Data")
	FName EdgeTopSocketName = NAME_None;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;

	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType() const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UAT_DamageTypeBase> GetDamageType() const;

	UFUNCTION(BlueprintCallable)
	float GetBaseDamage() const;

	UFUNCTION(BlueprintCallable)
	FVector GetWeaponEdge();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
