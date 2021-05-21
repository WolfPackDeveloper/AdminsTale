// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"
#include "AdminsTale/Actors/Weapon.h"

#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
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
	bIsDead = false;
	Target = nullptr;

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	
}

void ACharacterBase::DyingActionDelayed()
{
	GetWorldTimerManager().ClearTimer(DelayTimer);

	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
	{
		// ����� �� �������� �������� ��� ��������� ������. �� � ����� ����� ��������������.
		if (IsValid(MeleeWeapon))
		{
			MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
			MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// �����-�� ��������! ��� ������� ��� ���� � ������������� �� ����. ������ �� ��������.
		// �����, ����� ����� ������ - �� ��������� ��������.
		//GetMesh()->SetSimulatePhysics(true);
		// ���
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	//�������� ����� ���������� ���� � ������! ������ ��� �������� � ��������� ����...
	MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MeleeWeapon->SetOwner(this);

	// �������� ��������.
	if (Health)
	{
		Health->OnHealthEnded.AddDynamic(this, &ACharacterBase::OnHealthEnded);
	}

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

void ACharacterBase::EnableCombatMode(bool bEnable)
{
	bCombatMode = bEnable;
}

void ACharacterBase::SetCombatMode_Implementation()
{
	EnableCombatMode(!(bCombatMode));
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

void ACharacterBase::OnHealthEnded()
{
	
}

void ACharacterBase::DyingAction(class UAnimMontage* AnimMontage, float InPlayRate, float DelayTime)
{
	if (!bIsDead)
	{
		bIsDead = true;

		// �������� ��������� ������  ��� ������������.
		int32 AnimCount = AnimMontage->CompositeSections.Num() - 1;
		FName SectionName = AnimMontage->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;

		// ����� �� �������, ���� ������� �� ���� �� ����� ������.
		if (IsValid(GetMesh()))
		{
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}

		DetachFromControllerPendingDestroy();

		// ����������� ��� ��������
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance->IsAnyMontagePlaying())
		{
			UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();

			AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(Montage), Montage);
		}

		PlayAnimMontage(AnimMontage, InPlayRate, SectionName);

		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ACharacterBase::DyingActionDelayed, DelayTime, false);

		// ���������� � DyingActionDelayed - �������, �������. �� ������ ���������, ���-�� �� �����.
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
	// ��� ������� ������, ����� ����� ��� �������.

	
	return DamageMultiplier;
}

bool ACharacterBase::IsSneaking()
{
	return bSneaking;
}

bool ACharacterBase::IsDead()
{
	return bIsDead;
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
