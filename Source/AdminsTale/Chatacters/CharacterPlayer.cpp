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

	//Ставим "крепление" оружия в нормальное положение
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
	
	//Такие дела - ходим только "вперёд".
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//Вариант чуть поинтереснее
	bTargetMode = false;
	SetPlayerRotationMode();


	//Заморочки с приседом...
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
		// Чтобы крутить головой во время прцеливания нужно выключить эту функцию.
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
	// Крутимся вокруг цели
	// С условиями нужно подумать - bTargetEnemy?
	if (IsValid(Controller) && bCombatMode && IsValid(TargetedEnemy))
	{
		//Пока так, вроде бы "прицеливание" работает. Зачем сложности с миллионом ротаторов - пока не ясно.
		//Также мутно пока с кувырком - кувыркается только вперёд, но не ясно - это из-за того, что анимация такая (возможно, если будет боковой кувырок,
		//то ситуация выправится, но это не точно). Или же придётся переделывать получение правого вектора для кувырка.
		
		//float InterpSpeed = 5.f;
		//Направление взгляда? Чего бл...?!
		//FRotator ActorRotation = GetActorRotation();
		
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		
		//FRotator TargetRotation = FRotator(ActorRotation.Roll, ActorRotation.Pitch, LookAtRotation.Yaw);
		//FRotator CurrentRotation = FMath::RInterpTo(ActorRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		
		//Controller->SetControlRotation(CurrentRotation);

		// Тут головой не крутим - смотрим чётко в цель. Классика.
		Controller->SetControlRotation(LookAtRotation);
		
		// Чтобы можно было во время "прицеливания" головой вертеть (Darksiders 2 style).
		// Факапится направление движения, когда вращаешь камерой.
		// Потом подумаю, что да как. Скорее всего нужно логику инпутов менять.
		//SetActorRotation(LookAtRotation);
	}
}

// Called when the game starts or when spawned
void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//По умолчанию - бежим. А то медленно он ходит. Бесит.
	bRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	//Собацкая палка отправляла меня в космос! Потому что коллизии и фантомные силы...
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
	
	//Пока что тут, но, возможно имеет смысл перенести это в TurnRate.
	if (bTargetMode)
	{
		SetPlayerToTargetRotation();
	}
}

// Called to bind functionality to input
void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	//Походу привязываемый делегат не должен иметь параметров...
	//PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::CanRoll);
	//PlayerInputComponent->BindAction (TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::TargetEnemy);
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Released, this, &ACharacterPlayer::StopTargetingEnemy);
	
}