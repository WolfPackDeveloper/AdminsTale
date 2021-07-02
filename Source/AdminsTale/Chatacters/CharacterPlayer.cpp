// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"
#include "AdminsTale/Chatacters/CharacterEnemy.h"
//#include "AdminsTale/Actors/Weapon.h"

#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
//#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
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
	//bRunning = false;
	//bSprinting = false;
	//bSneaking = false;
	// ��� ���� � ������ ����� - ��������� ����� Run - � ��� ������ ������� Run() �������� ������.
	CurrentMovementStatus = EMovementStatus::Walk;
	bTargetMode = false;
	bCombatMode = false;

	saDefaultRelativeLocation = FVector(0.f, 20.f, 70.f);
	saDefaultRelativeRotation = FRotator(0.f, 0.f, 0.f);
	saDefaultLength = 450.f;
	saPitchSocketOffset = 60.f;
	saYawSocketOffset = 30.f;

	//saTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("saTimelineComponent"));

	// ��������� ������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = saDefaultLength;
	// � SetControlRotation
	//SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(saDefaultRelativeLocation);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	// � SetControlRotation
	//Camera->bUsePawnControlRotation = true;

	//������ "���������" ������ � ���������� ���������
	//FVector ComponentLocation = FVector(11.f, 3.f, 19.f);
	//FRotator ComponentRotation = FRotator(15.f, 180.f, 25.f);

	//MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	//MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));
	////MeleeWeaponUnarmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);

	////ComponentLocation = FVector(35.f, -19.f, 3.f);
	////ComponentRotation = FRotator(90.f, -15.f, 180.f);

	//MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	//MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	////MeleeWeaponArmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);
	
	// � BeginPlay() - � �� �� ��������, �������� ���, ������ �� ���������. � � ���� � ������, ��� ��� �������������.
	//SetPlayerRotationMode();

	//��������� � ��������...
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}

//UAbilitySystemComponent* ACharacterPlayer::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

void ACharacterPlayer::GetClosestEnemy(TSubclassOf<ACharacterEnemy> EnemyClass, float Range)
{
	const FVector TraceStart = GetActorLocation();
	// ����� ������ � ����� ����������� ��������� � ����� ������, ����� �� ����� ����������� (������, ��������) ���� � ������� ������ ����.
	const FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * TargetRange);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	// � ���������� ������� �������������� �������� ����� ��� (����� ������ � �������, ����� ������ ��� WorldStatic � ������ ������).
	// �� ��������� �� ������� ������� (��, ������, � ����� ����) ���������� ��� � ���� - � �� ����� ���.
	// ������ ����� ��������� ��������� �������, ��������� ����� ������ �����������.
	//ObjectTypeQuery1 - world static
	//ObjectTypeQuery2 - world dynamic
	//ObjectTypeQuery3 - pawn
	//ObjectTypeQuery4 - physics body
	//ObjectTypeQuery5 - vehicle
	//ObjectTypeQuery6 - destructible
	//ObjectTypeQuery7 - custom 1
	// �������� �������� ����� �������� ������ - ��� ���, ������ ����� ����� ��������� �����-������ �� ������������, �� ���� ������.
	// � ��, �� �������� ����� � �������, ��� � ������ ���������� � ��������� �������� ��� ������� ���������.

	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	
	//� ���� ����� ��������.
	FHitResult EnemyToTarget;

	// C ������� �����������
	//const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
	//	GetWorld(),
	//	TraceStart,
	//	TraceEnd,
	//	TargetRadius,
	//	ObjectTypes,
	//	false,
	//	ActorsToIgnore,
	//	EDrawDebugTrace::ForDuration,
	//	EnemyToTarget,
	//	true,
	//	FLinearColor::Red,
	//	FLinearColor::Green,
	//	2.0f
	//);

	// ������� �������.

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		TargetRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		EnemyToTarget,
		true
	);

	if (bHit)
	{
		TargetedEnemy = Cast<ACharacterEnemy>(EnemyToTarget.Actor);
	}
}

void ACharacterPlayer::SetPlayerRotationMode()
{
	if (bTargetMode)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// ����� ������� ������� �� ����� ����������� ����� ��������� ��� �������.
		//bUseControllerRotationYaw = true;
		// ��������� �������� ����� �� ����������� (��� AdvancedRotation)
		//SpringArm->bUsePawnControlRotation = false;
		// ����� ������ �������� �� ����.
		Camera->bUsePawnControlRotation = false;
		// ����� ��������� ������ ���� ���� ����������.)
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->CameraLagMaxDistance = 20.f;
		SpringArm->CameraRotationLagSpeed = 4.f;

		// ������� ������ � ��������� "������������" - ����� �� �����, ������ �����.)
		//if (IsValid(saTimelineComponent))
		//{
		//	saTimelineComponent->Play();
		//}

		// ��������� ���� ��� �������� �����������
		if (IsValid(Controller))
		{
			Controller->SetIgnoreLookInput(true);
			Controller->SetControlRotation(GetActorRotation());
		}
		Camera->SetWorldRotation(GetActorRotation());
		SpringArm->SetWorldRotation(GetActorRotation());
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;

		// ���� ������� ������� �� ����� ������������, ����� ��� ������ �� ���������� ��� � ��������� � ��������� ������.
		//const FRotator PlayerLineOfSight = GetActorRotation();
		//FRotator Rotation = FRotator(0.f, PlayerLineOfSight.Yaw, 0.f);
		//SetActorRotation(Rotation);
				
		// ����� � ������ �������� ����������� �����������. ����� ��� ��������� ����� ���� ������ ��������� ������� � ����� ��������������� ����� - ���� �����-��.
		SpringArm->bUsePawnControlRotation = true;
		Camera->bUsePawnControlRotation = true;
		// �����, ����� �� ��������� ���� �� ��� ���������.)
		SpringArm->bEnableCameraRotationLag = false;
		SpringArm->CameraLagMaxDistance = 5.f;
		SpringArm->CameraLagSpeed = 10.f;
		SpringArm->CameraRotationLagSpeed = 10.f;

		if (IsValid(Controller))
		{
			Controller->SetIgnoreLookInput(false);
			Controller->SetControlRotation(GetActorRotation());
		}

		//SpringArm->SetWorldRotation(Rotation);
		//Camera->SetWorldRotation(GetControlRotation());

		// �� ��������� ������������ - ����������� �� ��� ����.
		//if (IsValid(saTimelineComponent))
		//{
		//	saTimelineComponent->Reverse();
		//}
	}
}

void ACharacterPlayer::SimpleTargetLoking()
{
	// ������������ ���� ����� � ����.
	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		FRotator ToTargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		SetActorRotation(ToTargetRotation);

		Camera->SetWorldRotation(ToTargetRotation);

		Controller->SetControlRotation(ToTargetRotation);
	}

	// ��������� ��������� � �������� ������ ������� ����.


	// ��������� ������ ����.
}

void ACharacterPlayer::AdvancedTargetLocking(float DeltaTime)
{
	// ����� ������������ - ����� ������ ����
	// Darksiders II style - ����������� ������ � ����� ������ SpringArm.
	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		// ����� ����� ���������� � ��������, ��� ��������.
		float InterpSpeed = 8.f;
		float ControllerYawLag = 90.f;
		float SpringArmYawLag = 45.f;
		
		// ������������ ���� ����� � ����.
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		FRotator ToTargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		//float PlayerYaw = FMath::FInterpTo(GetActorRotation().Yaw, LookAtRotation.Yaw, DeltaTime, InterpSpeed);
		//FRotator ToTargetRotation = GetActorRotation();
		//ToTargetRotation.Yaw = PlayerYaw;

		SetActorRotation(ToTargetRotation);

		// ���������� ����� ������.
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), TargetedEnemy->GetActorLocation());
		Camera->SetWorldRotation(LookAtRotation);

		// ���������� ����������� �����������
		float DeltaControllerRotation = fabs(GetActorRotation().Yaw - GetControlRotation().Yaw);
		//������� �� ������� ����.
		if (DeltaControllerRotation > ControllerYawLag)
		{
			float DeltaLag = DeltaControllerRotation - ControllerYawLag;
			float DeltaYaw = GetActorRotation().Yaw + DeltaLag;

			//if (GetControlRotation().Yaw > GetActorRotation().Yaw)
			//{
			//	DeltaYaw = GetControlRotation().Yaw - DeltaLag;
			//}
			//else
			//{
			//	DeltaYaw = GetControlRotation().Yaw + DeltaLag;
			//}

			float ControllerYaw = FMath::FInterpTo(GetControlRotation().Yaw, DeltaYaw, DeltaTime, InterpSpeed);
			FRotator ControllerRotation = GetControlRotation();
			ControllerRotation.Yaw = DeltaYaw;

			Controller->SetControlRotation(ControllerRotation);
		}
		
		//���������� ��������� �����.
		float DeltaSARotation = fabs(GetActorRotation().Yaw - SpringArm->GetComponentRotation().Yaw);
		//������� �� ������� ����.
		if (DeltaSARotation > SpringArmYawLag)
		{
			float DeltaLag = DeltaControllerRotation - SpringArmYawLag;
			float DeltaYaw = SpringArm->GetComponentRotation().Yaw - DeltaLag;

			//if (SpringArm->GetComponentRotation().Yaw > GetActorRotation().Yaw)
			//{
			//	DeltaYaw = SpringArm->GetComponentRotation().Yaw - DeltaLag;
			//}
			//else
			//{
			//	DeltaYaw = SpringArm->GetComponentRotation().Yaw + DeltaLag;
			//}

			float saYaw = FMath::FInterpTo(SpringArm->GetComponentRotation().Yaw, DeltaYaw, DeltaTime, InterpSpeed);
			FRotator saRotation = SpringArm->GetComponentRotation();
			saRotation.Yaw = DeltaYaw;

			SpringArm->SetWorldRotation(saRotation);
		}
	}
}

//void ACharacterPlayer::UpdateSpringArmOffset(FVector Location)
//{
//	SpringArm->SocketOffset = Location;
//}

// Called when the game starts or when spawned
void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//�� ��������� - �����. � �� �������� �� �����. �����.
	
	SetMovementStatus(EMovementStatus::Run);
	
	//bRunning = true;
	//GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	//MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	////�������� ����� ���������� ���� � ������! ������ ��� �������� � ��������� ����...
	//MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//MeleeWeapon->SetOwner(this);
	
	EnemyBaseClass = ACharacterEnemy::StaticClass();

	SetPlayerRotationMode();
	//If we have a vector curve, bind it's graph to our update function
	//if (CurveSAVector && IsValid(saTimelineComponent))
	//{
	//	OnTimeVSAOffset.BindUFunction(this, FName("UpdateSpringArmOffset"));
	//	saTimelineComponent->AddInterpVector(CurveSAVector, OnTimeVSAOffset);
	//}

}

void ACharacterPlayer::MakeRoll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName)
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
		TargetedEnemy->SetTargetedState(true);
		SetPlayerRotationMode();
	}
}

void ACharacterPlayer::StopTargetingEnemy()
{
	bTargetMode = false;
	
	if (IsValid(TargetedEnemy))
	{
		TargetedEnemy->SetTargetedState(false);
		TargetedEnemy = nullptr;
	}
	
	SetPlayerRotationMode();
}

// Called every frame
void ACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		SimpleTargetLoking();
		//AdvancedTargetLocking(DeltaTime);
	}
	//Debug
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, FString::Printf(TEXT("Controller rotation is: %s"), *GetControlRotation().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Character rotation is: %s"), *GetActorRotation().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("SpringArm rotaion (World) is: %s"), *SpringArm->GetComponentRotation().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Camera rotaion (World) is: %s"), *Camera->GetComponentRotation().ToString()));

	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("SpringArm Location is: %s"), *SpringArm->GetComponentLocation().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("SpringArm length is: %f"), SpringArm->TargetArmLength));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Camera Locatin (World) is: %s"), *Camera->GetComponentLocation().ToString()));
}

// Called to bind functionality to input
void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	


	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterBase::MoveForvard);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterBase::MoveRight);

	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterBase::LookUpRate);
	//PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ACharacterBase::TurnRate);

	//PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ACharacterBase::Run);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sprint);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ACharacterBase::StopSprinting);
	//PlayerInputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sneak);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacterBase::StopJumping);
	
	//PlayerInputComponent->BindAction(TEXT ("CombatMode"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetCombatMode);
	
	//PlayerInputComponent->BindAction(TEXT("AttackFast"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackFast);
	//PlayerInputComponent->BindAction(TEXT("AttackStrong"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackStrong);
	//PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &ACharacterBase::Action);

	//PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::TargetEnemy);
	//PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Released, this, &ACharacterPlayer::StopTargetingEnemy);
}