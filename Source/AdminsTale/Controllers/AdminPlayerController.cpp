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

	if (InputComponent)
	{
		//InputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterBase::MoveForvard);
	//InputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterBase::MoveRight);

	//InputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterBase::LookUpRate);
	//InputComponent->BindAxis(TEXT("TurnRate"), this, &ACharacterBase::TurnRate);

	//InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ACharacterBase::Run);
	//InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sprint);
	//InputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sneak);
	//InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	//InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacterBase::StopJumping);
	//InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACharacterBase::Attack);
	//InputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ACharacterBase::Shoot);
	//InputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &ACharacterBase::StopShooting);
	}
}
