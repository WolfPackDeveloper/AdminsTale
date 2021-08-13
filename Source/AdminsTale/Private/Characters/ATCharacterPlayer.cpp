// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterPlayer.h"
#include "ATAimComponent.h"
#include "ATWeaponComponent.h"

//#include "Animation/AnimInstance.h" // Для проверки AnimInstance - в базовом классе.
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h" // Может, заменить на хэдер PlayerController?
#include "GameFramework/SpringArmComponent.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/KismetMathLibrary.h"

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

//void AATCharacterPlayer::TakeAim()
//{
//	
//	TArray<TEnumAsByte<EObjectTypeQuery>> AimObjectTypes;
//	AimObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);
//
//	// В настройках проекта предварительно создаётся новый тип (будет шестым в массиве, после всяких так WorldStatic и прочей лабуды).
//	// Первые шесть элементов создаются движком, остальное можно самому придумывать.
//	//ObjectTypeQuery1 - world static
//	//ObjectTypeQuery2 - world dynamic
//	//ObjectTypeQuery3 - pawn
//	//ObjectTypeQuery4 - physics body
//	//ObjectTypeQuery5 - vehicle
//	//ObjectTypeQuery6 - destructible
//	//ObjectTypeQuery7 - custom 1
//
//	TArray<AActor*> ActorsToIgnore;
//	ActorsToIgnore.Add(this);
//
//	TArray<FHitResult> AimHitResults;
//
//	const bool Hit = UKismetSystemLibrary::SphereTraceMultiForObjects(
//		GetWorld(),
//		GetActorLocation(),
//		GetActorLocation(),
//		AimRadius,
//		AimObjectTypes,
//		false,
//		ActorsToIgnore,
//		EDrawDebugTrace::ForDuration,
//		AimHitResults,
//		true,
//		FLinearColor::Red,
//		FLinearColor::Green,
//		2.0f
//	);
//		
//	if (Hit)
//	{
//		AimHitResults.Sort(
//			[this](const FHitResult& A, const FHitResult& B)
//			{
//				float fA = UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), A.Actor->GetActorLocation()), GetActorRotation()).Yaw);
//				float fB = UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), B.Actor->GetActorLocation()), GetActorRotation()).Yaw);
//				return fA < fB;
//			}
//		);
//
//		AimedActor = AimHitResults[0].GetActor();
//	}
//}

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

void AATCharacterPlayer::MeleeAttack()
{
	AimComponent->TakeAim();
	WeaponComponent->MeleeAttack();
}

void AATCharacterPlayer::RangeAttack()
{
	AimComponent->TakeAim();
	WeaponComponent->RangeAttack();
}

void AATCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AATCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
