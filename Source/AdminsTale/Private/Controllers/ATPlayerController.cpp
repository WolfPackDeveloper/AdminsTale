// Fill out your copyright notice in the Description page of Project Settings.


#include "ATPlayerController.h"
#include "Characters/ATCharacterPlayer.h"
#include "Components/ATWeaponComponent.h"

AATPlayerController::AATPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AATPlayerController::MoveForward(float AxisValue)
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->MoveForward(AxisValue);
	}
}

void AATPlayerController::MoveRight(float AxisValue)
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->MoveRight(AxisValue);
	}
}

void AATPlayerController::LookUpRate(float AxisValue)
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->LookUpRate(AxisValue);
	}
}

void AATPlayerController::TurnRightRate(float AxisValue)
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->TurnRightRate(AxisValue);
	}
}

void AATPlayerController::Jump()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Jump();
	}
}

void AATPlayerController::StopJumping()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->StopJumping();
	}
}

void AATPlayerController::MeleeAttack()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetWeaponComponent()->MeleeAttack();
	}
}

void AATPlayerController::RangeAttack()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetWeaponComponent()->RangeAttack();
	}
}

void AATPlayerController::Dash()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Dash();
	}
}

//void AATPlayerController::Aim()
//{
//	if (IsValid(PlayerCharacter))
//	{
//		PlayerCharacter->Aim();
//	}
//}

void AATPlayerController::SheatheWeapon()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetWeaponComponent()->EquipWeapon(nullptr);
	}
}

void AATPlayerController::Action()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Action();
	}
}

void AATPlayerController::Run()
{
	const EMovementBehaviour CurrentBehaviour = PlayerCharacter->GetMovementBehaviour();

	if (CurrentBehaviour == EMovementBehaviour::Run)
	{
		PlayerCharacter->SetMovementBehaviour(EMovementBehaviour::Walk);
	}
	else
	{
		PlayerCharacter->SetMovementBehaviour(EMovementBehaviour::Run);
	}
}

void AATPlayerController::Crouch()
{
	// В бою не приседаем. Это не по-пацански.
	//if (PlayerCharacter->IsInCombat())
	//{
	//	return;
	//}
	
	const EMovementBehaviour CurrentBehaviour = PlayerCharacter->GetMovementBehaviour();

	if (CurrentBehaviour == EMovementBehaviour::Crouch)
	{
		PlayerCharacter->SetMovementBehaviour(EMovementBehaviour::Run);
	}
	else
	{
		PlayerCharacter->SetMovementBehaviour(EMovementBehaviour::Crouch);
	}

}

void AATPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AATPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	PlayerCharacter = Cast<AATCharacterPlayer>(aPawn);
}

void AATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AATPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AATPlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookUpRate"), this, &AATPlayerController::LookUpRate);
	InputComponent->BindAxis(TEXT("TurnRightRate"), this, &AATPlayerController::TurnRightRate);

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AATPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AATPlayerController::StopJumping);
	InputComponent->BindAction(TEXT("MeleeAttack"), EInputEvent::IE_Pressed, this, &AATPlayerController::MeleeAttack);
	InputComponent->BindAction(TEXT("RangeAttackr"), EInputEvent::IE_Pressed, this, &AATPlayerController::RangeAttack);
	InputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AATPlayerController::Dash);
	//InputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AATPlayerController::Aim);
	InputComponent->BindAction(TEXT("SheatheWeapon"), EInputEvent::IE_Pressed, this, &AATPlayerController::SheatheWeapon);
	InputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &AATPlayerController::Action);

	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AATPlayerController::Run);
	InputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AATPlayerController::Crouch);
}

void AATPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
