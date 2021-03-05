// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AdminsTaleGameModeBase.h"
#include "Chatacters/CharacterPlayer.h"
#include "Controllers/AdminPlayerController.h"

// Called when the game starts or when spawned
void AAdminsTaleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Default settings
	DefaultPawnClass = ACharacterPlayer::StaticClass();
	PlayerControllerClass = AAdminPlayerController::StaticClass();
}