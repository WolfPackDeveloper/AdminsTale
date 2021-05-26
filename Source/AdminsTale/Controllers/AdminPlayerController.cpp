// Fill out your copyright notice in the Description page of Project Settings.


#include "AdminPlayerController.h"
#include "AdminsTale/Chatacters/CharacterBase.h"
#include "AdminsTale/Chatacters/CharacterPlayer.h"

#include "Components/InputComponent.h"

AAdminPlayerController::AAdminPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAdminPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAdminPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
