// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"
#include "AdminsTale/Chatacters/Components/ManaComponent.h"

#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Mana = CreateDefaultSubobject<UManaComponent>(TEXT("Mana"));
	
	BaseTurnRate = 70.f;
	SprintSpeed = 600.f;
	RunSpeed = 420.f;
	WalkSpeed = 300.f;
	CrouchSpeed = 120.f;
	IsInAir = false;
	IsRunning = false;
	IsSprinting = false;
	IsSneaking = false;
	
	ShotDistance = 2000.f;
	ImpulseForce = 500.f;
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
	//�� �� ���� �������� ���� ������ ��������... �� ����� ����� ���������???!!!
	if ((IsValid(Controller)) && (AxisValue != 0.0f))
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
	//AddMovementInput(GetActorRightVector() * AxisValue);
	
	if ((IsValid(Controller)) && (AxisValue != 0.0f))
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
	Super::Jump();
}

void ACharacterBase::Run()
{
	//�����, ��� ���
	IsRunning = !(IsRunning);
	//UE_LOG(LogTemp, Warning, TEXT("Run Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//�� ������ �� �������
	IsSneaking = false;
	IsSprinting = false;

	if (IsRunning)
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
	//���������, ��� �����
	IsSprinting = !(IsSprinting);
	//UE_LOG(LogTemp, Warning, TEXT("Sprint Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//�� ������ �� �������
	IsSneaking = false;
	IsRunning = false;

	if (IsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ACharacterBase::Sneak()
{
	// �������, ��� ���
	IsSneaking = !(IsSneaking);
	//UE_LOG(LogTemp, Warning, TEXT("Stealth Mode is %s"), IsSneaking ? TEXT("true") : TEXT("false"));
	// �� ����� �� �����
	IsRunning = false;
	IsSprinting = false;

	if (IsSneaking)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ACharacterBase::Attack()
{

}

void ACharacterBase::Shoot()
{
	//����������� � ������� � ��������� ����������.
	
	////���� ��� ����������� - ����, ��� ��������������� � BP.
	////TraceForward();
	////��� ������ �� ������� LineTrace �� ����.
	//FVector ShooterLocation;
	//FRotator ShooterRotation;
	//FHitResult ShotHit;

	////�������� �������� ��� ������� � ����������� ���������.
	//GetController()->GetPlayerViewPoint(ShooterLocation, ShooterRotation);

	////���������� ������ - ����� "�����������". ��� �������� ��������� �� ���� � ���� ������ � ������� �������� "������" ���������.
	////����� ��������� StartShootPoint - ��� ��������?
	//FVector StartShotPoint = ShooterLocation;
	//FVector EndShotPoint = StartShotPoint + (ShooterRotation.Vector() * ShotDistance);
	//
	////�������� "���������" - ����������� (collision) "������ �����" ���� (������ ���������) �������.
	//FCollisionQueryParams TraceParams;
	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	ShotHit,
	//	StartShotPoint,
	//	EndShotPoint,
	//	ECC_Visibility,
	//	TraceParams
	//);

	//if (bHit & ShotHit.GetActor()->IsRootComponentMovable())
	//{
	//	//����� �������� �� ���� (��������� ���, ��� ��� ��� ��� ����� ����...) �������� � ������� �����������!
	//	
	//	UMeshComponent* MeshComponent = Cast<UMeshComponent>(ShotHit.GetActor()->GetRootComponent());
	//	if (MeshComponent)
	//	{
	//		FVector ActorForward = GetActorForwardVector();
	//		MeshComponent->AddImpulse(ActorForward * ImpulseForce * MeshComponent->GetMass());
	//	}
	//}

}

void ACharacterBase::StopShooting()
{

}

void ACharacterBase::TraceForward_Implementation()
{
	//��� ������ �� ������� LineTrace �� ����.
	FVector ShooterLocation;
	FRotator ShooterRotation;
	FHitResult ShotHit;

	//�������� �������� ��� ������� � ����������� ���������.
	GetController()->GetPlayerViewPoint(ShooterLocation, ShooterRotation);

	//���������� ������ - ����� "�����������". ��� �������� ��������� �� ���� � ���� ������ � ������� �������� "������" ���������.
	//����� ��������� StartShootPoint - ��� ��������?
	FVector StartShotPoint = ShooterLocation;
	FVector EndShotPoint = StartShotPoint + (ShooterRotation.Vector() * ShotDistance);
	FVector HitSplash = FVector(5, 5, 5);

	//�������� "���������" - ����������� (collision) "������ �����" ���� (������ ���������) �������.
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		ShotHit,
		StartShotPoint,
		EndShotPoint,
		ECC_Visibility,
		TraceParams
	);
	//������������� ����������.
	DrawDebugLine(GetWorld(), StartShotPoint, EndShotPoint, FColor::Orange, false, 2.f, 0, 2.f);

	if (bHit)
	{
		DrawDebugBox(GetWorld(), ShotHit.ImpactPoint, HitSplash, FColor::Emerald, false, 2.f);
	}
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
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACharacterBase::Attack);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ACharacterBase::Shoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &ACharacterBase::StopShooting);
}
