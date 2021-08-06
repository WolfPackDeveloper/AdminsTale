// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterPlayer.h"

//#include "Animation/AnimInstance.h" // Для проверки AnimInstance - в базовом классе.
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h" // Может, заменить на хэдер PlayerController?
#include "GameFramework/SpringArmComponent.h"

AATCharacterPlayer::AATCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = saDefaultLength;
	SpringArm->SetRelativeLocation(saDefaultRelativeLocation);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AATCharacterPlayer::SetDefaultRotationBehaviour()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = true;
}

void AATCharacterPlayer::SetAimRotationBehaviour()
{

}

void AATCharacterPlayer::EnableAimRotation(bool EnableAimRotation)
{
	if (EnableAimRotation)
	{
		SetAimRotationBehaviour();
	}
	else
	{
		SetDefaultRotationBehaviour();
	}
}

void AATCharacterPlayer::TakeAim()
{
}

void AATCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	EnableAimRotation(false);

}

void AATCharacterPlayer::MoveForward(float AxisValue)
{
	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsMakingAction())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
}

void AATCharacterPlayer::MoveRight(float AxisValue)
{
	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsMakingAction())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, AxisValue);
	}
}

void AATCharacterPlayer::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AATCharacterPlayer::TurnRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AATCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AATCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
