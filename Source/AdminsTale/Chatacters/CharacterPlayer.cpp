// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"
#include "AdminsTale/Chatacters/CharacterEnemy.h"
#include "AdminsTale/Actors/Weapon.h"

#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
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
	bTargetMode = false;
	bCombatMode = false;

	saDefaultRelativeLocation = FVector(0.f, 20.f, 70.f);
	saDefaultRelativeRotation = FRotator(0.f, 0.f, 0.f);
	saDefaultLength = 450.f;
	saPitchSocketOffset = 100.f;
	saYawSocketOffset = 50.f;

	// Настройки камеры
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = saDefaultLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(saDefaultRelativeLocation);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Ставим "крепление" оружия в нормальное положение
	//FVector ComponentLocation = FVector(11.f, 3.f, 20.f);
	//FRotator ComponentRotation = FRotator(15.f, 180.f, 25.f);

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));
	//MeleeWeaponUnarmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);

	//ComponentLocation = FVector(35.f, -19.f, 3.f);
	//ComponentRotation = FRotator(90.f, -15.f, 180.f);

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	//MeleeWeaponArmed->SetRelativeLocationAndRotation(ComponentLocation, ComponentRotation);
	
	SetPlayerRotationMode();

	//Заморочки с приседом...
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}

//UAbilitySystemComponent* ACharacterPlayer::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

void ACharacterPlayer::GetClosestEnemy(TSubclassOf<ACharacterEnemy> EnemyClass, float Range)
{
	const FVector TraceStart = GetActorLocation();
	// Можно начало и конец трассировки назначить в точке актора, тогда он будет вылавливать (видимо, рандомно) цель в радиусе вокруг себя.
	const FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * TargetRange);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	// В настройках проекта предварительно создаётся новый тип (будет шестым в массиве, после всяких так WorldStatic и прочей лабуды).
	// По умолчанию до запуска проекта (да, блеать, и после тоже) посмотреть что к чему - я не понял как.
	// Первые шесть элементов создаются движком, остальное можно самому придумывать.
	//ObjectTypeQuery1 - world static
	//ObjectTypeQuery2 - world dynamic
	//ObjectTypeQuery3 - pawn
	//ObjectTypeQuery4 - physics body
	//ObjectTypeQuery5 - vehicle
	//ObjectTypeQuery6 - destructible
	//ObjectTypeQuery7 - custom 1
	// Возможно создание своих объектов излише - так как, скорее всего можно присвоить какой-нибудь из существующих, но есть нюансы.
	// И да, не забываем мобам в принтах, или в плюсах выставлять в категории коллизий тип объекта созданный.

	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);

	TArray<AActor*> ActorsToIgnore;
	// Это если создавать под это дело отдельный актор - что не лишено смысла, потому что можно повесить это на врагов тоже.
	ActorsToIgnore.Add(GetOwner());
	//В кого будем целиться.
	FHitResult EnemyToTarget;

	// C дебагом трассировки
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
	//	5.0f
	//);

	// Рабочий вариант.

	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
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

	if (Hit)
	{
		TargetedEnemy = Cast<ACharacterEnemy>(EnemyToTarget.Actor);
	}
}

void ACharacterPlayer::SetPlayerRotationMode()
{
	if (bTargetMode)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// Чтобы крутить головой во время прцеливания нужно выключить эту функцию.
		//bUseControllerRotationYaw = true;
		
		// Двигаем камеру в положение "прицеливание" - нафиг не нужно, просто блажь.)
		if (IsValid(saTimelineComponent))
		{
			saTimelineComponent->Play();
		}

		// Блокируем ввод для поворота контроллера
		if (IsValid(Controller))
		{
			Controller->SetIgnoreLookInput(true);
		}
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
		
		// По окончании прицеливания - возвращамем всё как было.
		if (IsValid(saTimelineComponent))
		{
			saTimelineComponent->Reverse();
		}
		
		if (IsValid(Controller))
		{
			Controller->SetIgnoreLookInput(false);
		}
	}
}

void ACharacterPlayer::SimpleTargetLoking()
{
	// Поворачиваем Чара лицом к цели.
	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		FRotator ToTargetYawRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		SetActorRotation(ToTargetYawRotation);
	}

	// Камеру и контроллер ставим в "дефолтное" положение.

	// Двигаемся вокруг цели.
}

void ACharacterPlayer::AdvancedTargetLocking()
{
	// Режим прицеливания - ходим вокруг цели
	// Darksiders II style - перемещение камеры с лагом длинны SpringArm.
	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		// Поворачиваем Чара лицом к цели.
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetedEnemy->GetActorLocation());
		FRotator ToTargetYawRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		SetActorRotation(ToTargetYawRotation);
		
		//LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpringArm->GetComponentLocation(), GetActorLocation());
		//SpringArm->SetWorldRotation(LookAtRotation);

		// Поворачиваем контроллер "лицом" к цели.
		// Играться для начала будем с камерой, а не контроллером.
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), TargetedEnemy->GetActorLocation());
		//ToTargetYawRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		Camera->SetWorldRotation(LookAtRotation);
		
		Controller->SetControlRotation(Camera->GetComponentRotation());

		// Указываем в градусах - при удалении чара от фокуса - увеличиваем, чтобы оба были на экране.
		// При сбросе фокуса - когда слишком далеко ушли, прекратили прицеливание, умерла цель - приводим к стандартному значению. Какое оно???
		// При приближении чара к фокусу - уменьшаем угол соответственно.
		//Camera->SetFieldOfView();

		// Проблема с возвращением камеры на место после прицеливания.
		// Проблема с следованием камеры за чаром - интерполяция локациии и ротации.
		// Отложим пока, а то так и кукухой поехать можно...
	}
}

void ACharacterPlayer::UpdateSpringArmOffset(FVector Location)
{
	SpringArm->SetWorldLocation(Location);
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

	//If we have a vector curve, bind it's graph to our update function
	if (CurveSAVector)
	{
		OnTimeVSAOffset.BindUFunction(this, FName("UpdateSpringArmOffset"));
		saTimelineComponent->AddInterpVector(CurveSAVector, OnTimeVSAOffset);
	}

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
		//Может быть здесь будм прицеливаться, м пожет и в SetPlayerRotationMode
		
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

	//Бля, ну хуй знает. Говнище какое-то, нужно просто в принтах сделать и не париться.(
	//CurveTimeline.TickTimeline(DeltaTime);

	if (bCombatMode && bTargetMode && IsValid(TargetedEnemy))
	{
		//SimpleTargetLoking();
		AdvancedTargetLocking();
	}
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
	//PlayerInputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed, this, &ACharacterBase::Sneak);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacterBase::StopJumping);

	//PlayerInputComponent->BindAction(TEXT("CombatMode"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetCombatMode);

	//PlayerInputComponent->BindAction(TEXT("AttackFast"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackFast);
	//PlayerInputComponent->BindAction(TEXT("AttackStrong"), EInputEvent::IE_Pressed, this, &ACharacterBase::AttackStrong);
	//PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &ACharacterBase::Action);
	
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterPlayer::MoveForvard);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterPlayer::MoveRight);
	//
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacterPlayer::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACharacterPlayer::AddControllerYawInput);
	//PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterPlayer::LookUpRate);
	//PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ACharacterPlayer::TurnRate);

	//Походу привязываемый делегат не должен иметь параметров...  Это пи...ж - доказано, см. выше. Тогда в чём дело?
	//PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::CanRoll);
	//PlayerInputComponent->BindAction (TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::Jump);
	
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Pressed, this, &ACharacterPlayer::TargetEnemy);
	PlayerInputComponent->BindAction(TEXT("TargetEnemy"), EInputEvent::IE_Released, this, &ACharacterPlayer::StopTargetingEnemy);
	
}