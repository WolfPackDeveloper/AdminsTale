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
#include "Engine/Engine.h" //Debug string.
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GAS отложен до лучших времён...
	//AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
		
	BaseTurnRate = 70.f;
	SprintSpeed = 500.f;
	RunSpeed = 350.f;
	WalkSpeed = 150.f;
	SneakSpeed = 150.f;
	
	CurrentMovementStatus = EMovementStatus::Walk;
	
	bIsDead = false;
	Target = nullptr;

	MeleeWeaponUnarmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponUnarmed"));
	MeleeWeaponUnarmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponUnarmedSocket"));

	MeleeWeaponArmed = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeWeaponArmed"));
	MeleeWeaponArmed->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MeleeWeaponArmedSocket"));
	
}

// Включение Ragdoll`а. Вопрос - а надо ли это вообще?
//void ACharacterBase::DyingActionDelayed()
//{
//	GetWorldTimerManager().ClearTimer(DelayTimer);
//
//	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
//	{
//		// Чтобы не коноёбило туловище при включении физики. Ну и чтобы пушка отстреливалась.
//		if (IsValid(MeleeWeapon))
//		{
//			MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//			MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
//			MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		}
//
//		GetMesh()->SetAllBodiesSimulatePhysics(true);
//		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		SetActorTickEnabled(false);
//	}
//}

void ACharacterBase::SpawnWeapon()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	
	if (IsValid(MeleeWeapon))
	{
		//MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		// Хмм, а есть привязка без физики?!
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, AttachmentRules);
		
		// Под вопросом
		MeleeWeapon->SetOwner(this);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	// Вынесено в отдельную функцию.
	//MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	////Собацкая палка отправляла меня в космос! Потому что коллизии и фантомные силы...
	//MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//MeleeWeapon->SetOwner(this);

	SpawnWeapon();

	// Привязка делегата.
	if (Health)
	{
		Health->OnHealthEnded.AddDynamic(this, &ACharacterBase::OnHealthEnded);
	}

} 

void ACharacterBase::MakeAttack(float AttackDamageMultiplier, UAnimMontage* AnimMontage, float PlayRate, FName StartSection)
{
	if (!IsValid(MeleeWeapon)) return;

	// Для чара костыль - SetCapsuleRadius = 70.f.
	// Это позволяет не бить мимо цели, если персонаж стоит слишком близко к нему.
	// TODO: Истребить костыль капсулы.
	//GetCapsuleComponent()->SetCapsuleRadius(70.f);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Пока что атака не прерывает никакие анимации.
	if (AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	DamageMultiplier = AttackDamageMultiplier;

	// Вынесено во внешнюю функцию.
	//if (!bCombatMode)
	//{
	//	bCombatMode = true;
	//	MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//}

	PlayAnimMontage(AnimMontage, PlayRate, StartSection);
}

void ACharacterBase::OnHit(bool bIsHitReaction)
{
	if (bIsHitReaction)
	{
		float BlendTime = 0.2f;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance->IsAnyMontagePlaying())
		{
			AnimInstance->StopAllMontages(BlendTime);
		}

		FName SectionName = MontageFastAttack->GetSectionName(0);

		PlayAnimMontage(MontageOnHit, OnHitPlayRate, SectionName);
	}

	if (!bCombatMode)
	{
		SetCombatMode(true);
	}
}

//void ACharacterBase::OnHit(bool bIsHitReaction, UAnimMontage* AnimMontage, float PlayRate, FName StartSection)
//{
//	//Это добро будет по цепочке дальше.
//	//if (!bCombatMode)
//	//{
//	//	bCombatMode = true;
//	//	MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	//}
//
//	if (bIsHitReaction)
//	{
//		float BlendTime = 0.2f;
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//		if (AnimInstance->IsAnyMontagePlaying())
//		{
//			AnimInstance->StopAllMontages(BlendTime);
//		}
//
//		PlayAnimMontage(AnimMontage, PlayRate, StartSection);
//	}
//}

void ACharacterBase::EquipWeapon(bool bEquip, float PlayRate)
{
	if (!IsValid(MontageEquipWeapon)) return;

	float BlendTime = 0.2f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying())
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentMontage == MontageOnHit)
		{
			AnimInstance->Montage_Stop(BlendTime, CurrentMontage);
		}
		else
		{
			return;
		}
	}

	// По идее здесь должно получаться 0 или 1.
	if (MontageEquipWeapon->IsValidSectionIndex(bEquip))
	{
		PlayAnimMontage(MontageEquipWeapon, PlayRate, MontageEquipWeapon->GetSectionName(bEquip));
	}
	else
	{
		//Debug
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error section index in EquipWeaponMontage.")));
	}	
}

void ACharacterBase::SetCombatMode(bool bEnableCombatMode)
{
	// Привязка оружия к сокету теперь через AnimNotify.
	bCombatMode = bEnableCombatMode;

	// Здесь может быть ещё какая-нибудь логика, доселе неизведанная.

	EquipWeapon(bCombatMode, EquipWeaponPlayRate);
}

void ACharacterBase::OnHealthEnded()
{

}

//void ACharacterBase::DyingAction(class UAnimMontage* AnimMontage, float InPlayRate, float DelayTime)
//{
//	if (!bIsDead)
//	{
//		bIsDead = true;
//
//		// Выбираем рандомную секцию  для проигрывания.
//		int32 AnimCount = AnimMontage->CompositeSections.Num() - 1;
//		FName SectionName = AnimMontage->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;
//
//		// Чтобы не дёргался, если получит по рылу во время смерти.
//		if (IsValid(GetMesh()))
//		{
//			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//		}
//
//		DetachFromControllerPendingDestroy();
//
//		// Заканчиваем все анимации
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//		if (AnimInstance->IsAnyMontagePlaying())
//		{
//			//UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
//
//			//AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(Montage), Montage);
//			float BlendTime = 0.2f;
//			AnimInstance->StopAllMontages(BlendTime);
//		}
//
//		PlayAnimMontage(AnimMontage, InPlayRate, SectionName);
//
//		// Теперь по AnimNotify. Нахрен всякие там Delay.
//		//GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ACharacterBase::DyingActionDelayed, DelayTime, false);
//	}
//}

void ACharacterBase::OnDeathStart()
{
	if (!bIsDead)
	{
		bIsDead = true;

		// Выбираем рандомную секцию  для проигрывания. Подразумевается, что анимакция помирания не одна и выбирается рандомно.
		int32 AnimCount = MontageOnDeath->CompositeSections.Num() - 1;
		FName SectionName = MontageOnDeath->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;

		// Чтобы не дёргался, если получит по рылу во время смерти.
		if (IsValid(GetMesh()))
		{
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}

		DetachFromControllerPendingDestroy();

		// Заканчиваем все анимации
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance->IsAnyMontagePlaying())
		{
			//UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();

			//AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(Montage), Montage);
			float BlendTime = 0.2f;
			AnimInstance->StopAllMontages(BlendTime);
		}

		PlayAnimMontage(MontageOnDeath, OnDeathPlayRate, SectionName);
	}
}

void ACharacterBase::OnDeathEnd()
{
	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
	{
		// Чтобы не коноёбило туловище при включении физики. Ну и чтобы пушка отстреливалась.
		if (IsValid(MeleeWeapon))
		{
			MeleeWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeleeWeapon->GetMesh()->SetSimulatePhysics(true);
			MeleeWeapon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// А если выключить этот кривой Ragdoll?
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
	}
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
	if (CurrentMovementStatus == EMovementStatus::Run)
	{
		CurrentMovementStatus = EMovementStatus::Walk;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		CurrentMovementStatus = EMovementStatus::Run;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ACharacterBase::Sprint()
{
	CurrentMovementStatus = EMovementStatus::Sprint;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterBase::StopSprinting()
{
	CurrentMovementStatus = EMovementStatus::Run;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ACharacterBase::Sneak()
{
	if (CurrentMovementStatus == EMovementStatus::Sneack)
	{
		CurrentMovementStatus = EMovementStatus::Walk;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		CurrentMovementStatus = EMovementStatus::Sneack;
		GetCharacterMovement()->MaxWalkSpeed = SneakSpeed;
	}
}

void ACharacterBase::Walk()
{
	CurrentMovementStatus = EMovementStatus::Walk;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACharacterBase::MakeRoll(UAnimMontage* RollAnimMontage, float RollPlayRate, FName RollSectionName)
{

}

void ACharacterBase::Roll_Implementation()
{
}

// Triggered by AnimNotify - AttachWeapon.
void ACharacterBase::AttachWeapon(bool bIsEquip)
{
	if (bIsEquip)
	{
		MeleeWeapon->AttachToComponent(MeleeWeaponArmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		MeleeWeapon->AttachToComponent(MeleeWeaponUnarmed, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACharacterBase::SwitchCombatMode_Implementation()
{
	if (!IsValid(MeleeWeapon)) return;
	SetCombatMode(!bCombatMode);
}

void ACharacterBase::AttackFast_Implementation()
{
	if (!bCombatMode)
	{
		SwitchCombatMode();
		//SetCombatMode(true);
	}
	
	// Это делается внутри функции MakeAttack.
	//DamageMultiplier = FastAttackDamageMultiplier;

	// Если в дальнейшем будут комбо, то тут будет логика выбора секции для проигрывания.
	
	// ...

	FName SectionName = MontageFastAttack->GetSectionName(0);

	MakeAttack(FastAttackDamageMultiplier, MontageFastAttack, FastAttackPlayRate, SectionName);
}

void ACharacterBase::AttackStrong_Implementation()
{
	if (!bCombatMode)
	{
		SwitchCombatMode();
		//SetCombatMode(true);
	}

	// Это делается внутри функции MakeAttack.
	//DamageMultiplier = FastAttackDamageMultiplier;

	// Если в дальнейшем будут комбо, то тут будет логика выбора секции для проигрывания.

	// ...

	FName SectionName = MontageStrongAttack->GetSectionName(0);

	MakeAttack(StrongAttackDamageMultiplier, MontageStrongAttack, StrongAttackPlayRate, SectionName);
}

void ACharacterBase::Block_Implementation()
{

}

void ACharacterBase::TargetEnemy()
{

}

void ACharacterBase::StopTargetingEnemy()
{
}

void ACharacterBase::Action()
{

}

//UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent;
//}

float ACharacterBase::CalculateDamageMultiplier()
{
	// Тут считаем всякое, когда будет что считать.

	
	return DamageMultiplier;
}

EMovementStatus ACharacterBase::GetMovementStatus() const
{
	return CurrentMovementStatus;
}

void ACharacterBase::SetMovementStatus(EMovementStatus MovementStatus)
{
	switch (MovementStatus)
	{
	case EMovementStatus::Sneack:
		Sneak();
		break;
	case EMovementStatus::Walk:
		Walk();
		break;
	case EMovementStatus::Run:
		Run();
		break;
	case EMovementStatus::Sprint:
		Sprint();
		break;
	default:
		break;
	}
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
}
