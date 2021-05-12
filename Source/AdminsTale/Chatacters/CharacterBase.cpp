// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"
#include "AdminsTale/Actors/Weapon.h"

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

	// GAS отложен до лучших времён...
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

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	//Собацкая палка отправляла меня в космос! Потому что коллизии и фантомные силы...
	MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MeleeWeapon->SetOwner(this);

} 

void ACharacterBase::MoveForvard(float AxisValue)
{
	//Вариант №1 - его предлагает пиздливый улыбчивый неорганизованный пёс.
	//AddMovementInput(GetActorForwardVector() * AxisValue);
	
	//Вариант №2 - внезапно нашёл на ютубе, его предлагает бесящий организованный бормотун.
	//Он, по идее, толковые вещи обычно бормочет... Но нахуя такие сложности???!!!
	
	//Останавливаем персонажа, во время действий (атака, кувырок и всякое такое)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	
	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//Почему мне пришлось контроллер прописывать в инклуды, а в примере и так хавает???
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	//Останавливаем персонажа, во время действий (атака, кувырок и всякое такое)
	bool IsInAction = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

	if (IsValid(Controller) && (AxisValue != 0.0f) && !IsInAction)
	{
		//Почему мне пришлось контроллер прописывать в инклуды, а в примере и так хавает???
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
	//Бежим, или идём
	bRunning = !(bRunning);
	//UE_LOG(LogTemp, Warning, TEXT("Run Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//Но только не крадёмся
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
		//Спринтуем, или бежим
		bSprinting = !(bSprinting);
		//Но только не крадёмся
		bSneaking = false;
		//Когда мы спринтуем, мы всё ещё бежим, логично же.))
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
		//В бою - только бегаем, ибо нех.
		// Такое себе решение, но лучше пока ничего не придумал...
		// А вообще все эти мутки со скоростью перемещения - баловство.
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
		// Крадёмся, или идём
		bSneaking = !(bSneaking);
		//UE_LOG(LogTemp, Warning, TEXT("Stealth Mode is %s"), IsSneaking ? TEXT("true") : TEXT("false"));
		// Но точно не бежим
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
		//В бою - только бегаем, ибо нех.
		// Такое себе решение, но лучше пока ничего не придумал...
		// А вообще все эти мутки со скоростью перемещения - баловство.
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
		
		// Пока не понятно, как лучше - анимация доставания оружия всё равно не проигрывается, да и долго...
		//EnableBattleMode();
	}
}

void ACharacterBase::AttackStrong_Implementation()
{
	if (!bCombatMode)
	{
		bCombatMode = true;
		
		// Пока не понятно, как лучше - анимация доставания оружия всё равно не проигрывается, да и долго...
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
	// Тут считаем всякое, когда будет что считать.

	
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
