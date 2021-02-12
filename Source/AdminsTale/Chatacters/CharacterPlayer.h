// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdminsTale/Chatacters/CharacterBase.h"
#include "CharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ADMINSTALE_API ACharacterPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:

	ACharacterPlayer();
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
