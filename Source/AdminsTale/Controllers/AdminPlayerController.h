// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AdminPlayerController.generated.h"

class ACharacterPlayer;
class UInputComponent;

UCLASS()
class ADMINSTALE_API AAdminPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	AAdminPlayerController();

private:

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

public:

};
