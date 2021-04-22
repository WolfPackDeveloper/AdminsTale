// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"
#include "AdminsTale/Chatacters/CharacterEnemy.h"
#include "AdminsTale/Actors/Weapon.h"

#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"


ACharacterPlayer::ACharacterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default values
	
	//AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	
	BaseTurnRate = 70.f;
	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	bRunning = false;
	bSprinting = false;
	bSneaking = false;

	FVector saLocation;
	saLocation.Set(0.f, 20.f, 70.f);
	float saArmLength = 450.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = saArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(saLocation);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//������ "���������" ������ � ���������� ���������
	//FVector ComponentLocation = FVector(11.f, 3.f, 19.f);
	//FRotator ComponentRotation = FRotator(15.f, 180.f, 25.f);

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));
	//MeleeWeaponUnarmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);

	//ComponentLocation = FVector(35.f, -19.f, 3.f);
	//ComponentRotation = FRotator(90.f, -15.f, 180.f);

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	//MeleeWeaponArmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);
	
	//����� ���� - ����� ������ "�����".
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//������� ���� ������������
	bTargetMode = false;
	SetPlayerRotationMode();


	//��������� � ��������...
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}

//UAbilitySystemComponent* ACharacterPlayer::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

//void ACharacterPlayer::Jump()
//{
//	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
//	{
//		Super::Jump();
//	}
//}

void ACharacterPlayer::GetClosestEnemy(TSubclassOf<ACharacterEnemy> EnemyClass, float Range)
{
	TArray<AActor*> FoundEnemies;
	float ClosestEnemyDistance = Range;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, FoundEnemies);

	for (AActor* Enemy : FoundEnemies)
	{
		if (Enemy->GetDistanceTo(this) < ClosestEnemyDistance)
		{
			ClosestEnemyDistance = Enemy->GetDistanceTo(this);
			TargetedEnemy = Cast<ACharacterEnemy>(Enemy);
		}
	}
}

void ACharacterPlayer::SetPlayerRotationMode()
{
	if (bTargetMode)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// ����� ������� ������� �� ����� ����������� ����� ��������� ��� �������.
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bUseControllerRotationYaw = false;
	}
}

void ACharacterPlayer::SetPlayerToTargetRotation()
{
	// �������� ������ ����
	// � ��������� ����� �������� - bTargetEnemy?
	if (IsValid(Controller) && bCombatMode && IsValid(TargetedEnemy))
	{
		//���� ���, ����� �� "������������" ��������. ����� ��������� � ��������� ��������� - ���� �� ����.
		//����� ����� ���� � �������� - ����������� ������ �����, �� �� ���� - ��� ��-�� ����, ��� �������� ����� (��������, ���� ����� ������� �������,
		//�� �������� ����������, �� ��� �� �����). ��� �� ������� ������������ ��������� ������� ������� ��� �������.
		
		//float InterpSpeed = 5.f;
		//����������� �������? ���� ��...?!
		//FRotator ActorRotation = GetActorRotation();
		
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		
		//FRotator TargetRotation = FRotator(ActorRotation.Roll, ActorRotation.Pitch, LookAtRotation.Yaw);
		//FRotator CurrentRotation = FMath::RInterpTo(ActorRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		
		//Controller->SetControlRotation(CurrentRotation);

		// ��� ������� �� ������ - ������� ����� � ����. ��������.
		Controller->SetControlRotation(LookAtRotation);
		
		// ����� ����� ���� �� ����� "������������" ������� ������� (Darksiders 2 style).
		// ��������� ����������� ��������, ����� �������� �������.
		// ����� �������, ��� �� ���. ������ ����� ����� ������ ������� ������.
		//SetActorRotation(LookAtRotation);
	}
}

// Called when the game starts or when spawned
void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//�� ��������� - �����. � �� �������� �� �����. �����.
	bRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	//�������� ����� ���������� ���� � ������! ������ ��� �������� � ��������� ����...
	MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MeleeWeapon->SetOwner(this);
	
	EnemyBaseClass = ACharacterEnemy::StaticClass();

	//PlayerHUD = CreateWidget(this, PlayerHUDClass);
	//if (PlayerHUD != nullptr)
	//{
	//	PlayerHUD->AddToViewport();
	//}
}

void ACharacterPlayer::Roll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName)
{
	if (!(GetCharacterMovement()->IsFalling()) && (GetCharacterMovement()->GetLastInputVector() != FVector(0.f,0.f,0.f)))
	{
		if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			UAnimMontage* CurrentMontage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
			
			if (CurrentMontage == RollAnimMontage)
			{
				return;
			}

			GetMesh()->GetAnimInstance()->Montage_Stop(0.f, CurrentMontage);
		}

		PlayAnimMontage(RollAnimMontage, RollPlayRate, RollSectionName);
	}
}

void ACharacterPlayer::TargetEnemy()
{
	GetClosestEnemy(EnemyBaseClass, TargetRange);

	if (IsValid(TargetedEnemy))
	{
		bTargetMode = true;
		SetPlayerRotationMode();
	}
}

void ACharacterPlayer::StopTargetingEnemy()
{
	bTargetMode = false;
	SetPlayerRotationMode();
	TargetedEnemy = nullptr;
}

// Called every frame
void ACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//���� ��� ���, ��, �������� ����� ����� ��������� ��� � TurnRate.
	if (bTargetMode)
	{
		SetPlayerToTargetRotation();
	}
}

// Called to bind functionality to input
void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	//������ ������������� ������� �� ������ ����� ����������...
	//PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::CanRoll);
	//PlayerInputComponent->BindAction (TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::TargetEnemy);
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Released, this, &ACharacterPlayer::StopTargetingEnemy);
	
}