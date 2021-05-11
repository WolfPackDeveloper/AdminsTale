// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCapsuleComponent;

UCLASS()
class ADMINSTALE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;

	// Чтобы наносить урон однокпратно за атаку
	bool bStrikes = false;

	FName SocketEdgeTop;
	FName SocketEdgeBottom;

	float CalculateDamage(float DamageMultiplier);

protected:
	
	// На будущее...
	UPROPERTY(EditAnywhere, Category = "Damage")
	float MinDamage = 0;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
	float MaxDamage = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetMesh() const;

	UFUNCTION(BlueprintCallable)
	void SetDamageEdge(FName EdgeTopSocket, FName EdgeBotomSocket);

	UFUNCTION(BlueprintCallable)
	void DealDamage();
	
	UFUNCTION(BlueprintCallable)
	void StopDamage();
};
