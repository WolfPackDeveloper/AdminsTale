// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "AdminsTale/Chatacters/Components/HealthComponent.h"
#include "AdminsTale/Chatacters/Components/ManaComponent.h"

#include "AbilitySystemComponent.h"
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

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Mana = CreateDefaultSubobject<UManaComponent>(TEXT("Mana"));
	
	BaseTurnRate = 70.f;
	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	IsRunning = false;
	IsSprinting = false;
	IsSneaking = false;
	
	//ShotDistance = 2000.f;
	//ImpulseForce = 500.f;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

} 

void ACharacterBase::MoveForvard(float AxisValue)
{
	//Вариант №1 - его предлагает пиздливый убывчивый неорганизованный пёс.
	//AddMovementInput(GetActorForwardVector() * AxisValue);
	
	//Вариант №2 - внезапно нашёл на ютубе, его предлагает бесящий организованный бормотун.
	//Он по идее толковые вещи обычно бормочет... Но нахуя такие сложности???!!!
	if ((IsValid(Controller)) && (AxisValue != 0.0f))
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
	//AddMovementInput(GetActorRightVector() * AxisValue);
	
	if ((IsValid(Controller)) && (AxisValue != 0.0f))
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
	Super::Jump();
}

void ACharacterBase::Run()
{
	//Бежим, или идём
	IsRunning = !(IsRunning);
	//UE_LOG(LogTemp, Warning, TEXT("Run Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//Но только не крадёмся
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
	//Спринтуем, или бежим
	IsSprinting = !(IsSprinting);
	//UE_LOG(LogTemp, Warning, TEXT("Sprint Mode is %s"), IsRunning ? TEXT("true") : TEXT("false"));
	//Но только не крадёмся
	IsSneaking = false;
	//Когда мы спринтуем, мы всё ещё бежим, логично же.))
	//IsRunning = false;

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
	// Крадёмся, или идём
	IsSneaking = !(IsSneaking);
	//UE_LOG(LogTemp, Warning, TEXT("Stealth Mode is %s"), IsSneaking ? TEXT("true") : TEXT("false"));
	// Но точно не бежим
	IsRunning = false;
	IsSprinting = false;

	if (IsSneaking)
	{
		GetCharacterMovement()->MaxWalkSpeed = SneakSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ACharacterBase::EnableBattleMode_Implementation()
{

}

void ACharacterBase::DisableBattleMode_Implementation()
{

}

void ACharacterBase::SetBattleMode_Implementation()
{
	IsBattleModeOn = !(IsBattleModeOn);

	if (IsBattleModeOn)
	{
		DisableBattleMode();
	}
	else
	{
		EnableBattleMode();
	}
}

void ACharacterBase::AttackFast_Implementation()
{
	if (!IsBattleModeOn)
	{
		IsBattleModeOn = true;
		
		// Пока не понятно, как лучше - анимация доставания оружия всё равно не проигрывается, да и долго...
		//EnableBattleMode();
	}
}

void ACharacterBase::AttackStrong_Implementation()
{
	if (!IsBattleModeOn)
	{
		IsBattleModeOn = true;
		
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

bool ACharacterBase::GetIsRunning()
{
	return IsRunning;
}

bool ACharacterBase::GetIsSprinting()
{
	return IsSprinting;
}

bool ACharacterBase::GetIsSneaking()
{
	return IsSneaking;
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
	
	PlayerInputComponent->BindAction(TEXT ("BattleMode"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetBattleMode);
	
	PlayerInputComponent->BindAction(TEXT("AttackFast"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackFast);
	PlayerInputComponent->BindAction(TEXT("AttackStrong"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackStrong);
	PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &ACharacterBase::Action);

}
