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

	PlayerCharacter = Cast<ACharacterPlayer>(GetPawn());
}

void AAdminPlayerController::MoveForvard(float AxisValue)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->MoveForvard(AxisValue);
}

void AAdminPlayerController::MoveRight(float AxisValue)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->MoveRight(AxisValue);
}

void AAdminPlayerController::LookUpRate(float AxisValue)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->LookUpRate(AxisValue);
}

void AAdminPlayerController::TurnRate(float AxisValue)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->TurnRate(AxisValue);
}

void AAdminPlayerController::Jump()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Jump();
}

void AAdminPlayerController::StopJumping()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->StopJumping();
}

void AAdminPlayerController::Run()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Run();
}

void AAdminPlayerController::Sprint()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Sprint();
}

void AAdminPlayerController::StopSprinting()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->StopSprinting();
}

void AAdminPlayerController::Sneak()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Sneak();
}

void AAdminPlayerController::Walk()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Walk();
}

void AAdminPlayerController::Roll()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Roll();
}

void AAdminPlayerController::Action()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Action();
}

void AAdminPlayerController::SwitchCombatMode()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->SwitchCombatMode();
}

void AAdminPlayerController::AttackFast()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->AttackFast();
}

void AAdminPlayerController::AttackStrong()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->AttackStrong();
}

void AAdminPlayerController::TargetEnemy()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->TargetEnemy();
}

void AAdminPlayerController::StopTargetingEnemy()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->StopTargetingEnemy();
}

void AAdminPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AAdminPlayerController::MoveForvard);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AAdminPlayerController::MoveRight);

	InputComponent->BindAxis(TEXT("LookUp"), this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis(TEXT("Turn"), this, &APlayerController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookUpRate"), this, &AAdminPlayerController::LookUpRate);
	InputComponent->BindAxis(TEXT("TurnRate"), this, &AAdminPlayerController::TurnRate);

	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::Run);
	InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::Sprint);
	InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AAdminPlayerController::StopSprinting);
	InputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::Sneak);
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AAdminPlayerController::StopJumping);
	InputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Released, this, &AAdminPlayerController::Roll);

	InputComponent->BindAction(TEXT("CombatMode"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::SwitchCombatMode);

	InputComponent->BindAction(TEXT("AttackFast"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::AttackFast);
	InputComponent->BindAction(TEXT("AttackStrong"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::AttackStrong);
	InputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::Action);

	InputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Pressed, this, &AAdminPlayerController::TargetEnemy);
	InputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Released, this, &AAdminPlayerController::StopTargetingEnemy);
}