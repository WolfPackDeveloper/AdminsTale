// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
	IsRunning = false;
	IsSprinting = false;
	IsSneaking = false;

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

	//Такие дела - ходим только "вперёд".
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Заморочки с приседом...
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}



//UAbilitySystemComponent* ACharacterPlayer::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

// Called when the game starts or when spawned
void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//По умолчанию - бежим. А то медленно он ходит. Бесит.
	IsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

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
		//FVector LastInputVector = GetCharacterMovement()->GetLastInputVector();
		PlayAnimMontage(RollAnimMontage, RollPlayRate, RollSectionName);
	}
}

// Called every frame
void ACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	//Походу привязываемый делегат не должен иметь параметров...
	//PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::CanRoll);
}