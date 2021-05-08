// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"

#include "AbilitySystemComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GAS ������� �� ������ �����...
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	
	BaseTurnRate = 70.f;
	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	bRunning = false;
	bSprinting = false;
	bSneaking = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

} 

void ACharacterBase::MoveForvard(float AxisValue)
{
	//������� �1 - ��� ���������� ��������� ��������� ���������������� ��.
	//AddMovementInput(GetActorForwardVector() * AxisValue);
	
	//������� �2 - �������� ����� �� �����, ��� ���������� ������� �������������� ��������.
	//��, �� ����, �������� ���� ������ ��������... �� ����� ����� ���������???!!!
	
	//������������� ���������, �� ����� �������� (�����, ������� � ������ �����)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	
	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//������ ��� �������� ���������� ����������� � �������, � � ������� � ��� ������???
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	//������������� ���������, �� ����� �������� (�����, ������� � ������ �����)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//������ ��� �������� ���������� ����������� � �������, � � ������� � ��� ������???
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::Jump()
{
	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		Super::Jump();
	}
}

void ACharacterBase::Run()
{
	//�����, ��� ���
	bRunning = !(bRunning);
	//UE_LOG(LogTemp, Warning, TEXT("Run Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//�� ������ �� �������
	bSneaking = false;
	bSprinting = false;

	if (bRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ACharacterBase::Sprint()
{
	if (!bCombatMode)
	{
		//���������, ��� �����
		bSprinting = !(bSprinting);
		//�� ������ �� �������
		bSneaking = false;
		//����� �� ���������, �� �� ��� �����, ������� ��.))
		//IsRunning = false;
		if (bSprinting)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
	}
	else
	{
		//� ��� - ������ ������, ��� ���.
		// ����� ���� �������, �� ����� ���� ������ �� ��������...
		// � ������ ��� ��� ����� �� ��������� ����������� - ���������.
		bRunning = true;
		bSneaking = false;
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ACharacterBase::Sneak()
{
	if (!bCombatMode)
	{
		// �������, ��� ���
		bSneaking = !(bSneaking);
		//UE_LOG(LogTemp, Warning, TEXT("Stealth Mode is %s"), IsSneaking ? TEXT("true") : TEXT("false"));
		// �� ����� �� �����
		bRunning = false;
		bSprinting = false;

		if (bSneaking)
		{
			GetCharacterMovement()->MaxWalkSpeed = SneakSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else
	{
		//� ��� - ������ ������, ��� ���.
		// ����� ���� �������, �� ����� ���� ������ �� ��������...
		// � ������ ��� ��� ����� �� ��������� ����������� - ���������.
		bRunning = true;
		bSneaking = false;
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ACharacterBase::CombatModeOn_Implementation()
{

}

void ACharacterBase::CombatModeOff_Implementation()
{

}

void ACharacterBase::SetCombatMode_Implementation()
{
	bCombatMode = !(bCombatMode);

	if (bCombatMode)
	{
		CombatModeOn();
	}
	else
	{
		CombatModeOff();
	}
}

void ACharacterBase::AttackFast_Implementation()
{
	if (!bCombatMode)
	{
		bCombatMode = true;
		
		// ���� �� �������, ��� ����� - �������� ���������� ������ �� ����� �� �������������, �� � �����...
		//EnableBattleMode();
	}
}

void ACharacterBase::AttackStrong_Implementation()
{
	if (!bCombatMode)
	{
		bCombatMode = true;
		
		// ���� �� �������, ��� ����� - �������� ���������� ������ �� ����� �� �������������, �� � �����...
		//EnableBattleMode();
	}
}

void ACharacterBase::Action()
{

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ACharacterBase::CalculateDamageMultiplier()
{
	// ��� ������� ������, ����� �����.
	
	return DamageMultiplier;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterBase::MoveForvard);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterBase::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ACharacterBase::TurnRate);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ACharacterBase::Run);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sneak);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacterBase::StopJumping);
	
	PlayerInputComponent->BindAction(TEXT ("CombatMode"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetCombatMode);
	
	PlayerInputComponent->BindAction(TEXT("AttackFast"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackFast);
	PlayerInputComponent->BindAction(TEXT("AttackStrong"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackStrong);
	PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &ACharacterBase::Action);

}
