// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AdminsTale/Chatacters/CharacterBase.h"
#include "CharacterEnemy.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class ADMINSTALE_API ACharacterEnemy : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	
	ACharacterEnemy();

private:

	// Set character rotation to targeted Actor.
	UFUNCTION()
	void SetTarget(AActor* TargetActor);

protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* BattleText = nullptr;

	// Is Character targeted by enemy.
	bool bTargeted = false;

	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Делаем видимый HealthBar и метку.
	UFUNCTION(BlueprintCallable)
	void SetTargetedState(bool TargetState);

};
