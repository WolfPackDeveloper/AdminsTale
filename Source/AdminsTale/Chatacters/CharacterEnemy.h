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

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* BattleText = nullptr;

	bool bTargeted = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetTargetedState(bool TargetState);

};
