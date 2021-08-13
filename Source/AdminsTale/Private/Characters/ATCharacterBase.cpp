// Fill out your copyright notice in the Description page of Project Settings.


#include "ATCharacterBase.h"
#include "Actors/ATWeaponBase.h"
#include "Components/ATAimComponent.h"
#include "Components/ATHealthComponent.h"
#include "Components/ATPowerComponent.h"
#include "Components/ATWeaponComponent.h"
#include "Objects/Damage/AT_DamageTypeBase.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h" // ApplyDamage
//#include "Kismet/KismetMathLibrary.h" // Math, LookAt

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
AATCharacterBase::AATCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AimComponent = CreateDefaultSubobject<UATAimComponent>(TEXT("AimComponent"));
	HealthComponent = CreateDefaultSubobject<UATHealthComponent>(TEXT("HealthComponent"));
	PowerComponent = CreateDefaultSubobject<UATPowerComponent>(TEXT("PowerComponent"));
	WeaponComponent = CreateDefaultSubobject<UATWeaponComponent>(TEXT("WeaponComponent"));

	// Set CollisionProfile for implementation damage events.
	FName MeshCollisionProfile = "Character";
	GetMesh()->SetCollisionProfileName(MeshCollisionProfile);
}

void AATCharacterBase::PlayOnHitMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (bCanReactOnHit)
	{
		if (AnimInstance->IsAnyMontagePlaying())
		{
			float BlendTime = 0.2f;
			AnimInstance->StopAllMontages(BlendTime);
		}

		PlayAnimMontage(MontageOnDie, OnDiePlayRate);
	}
}

void AATCharacterBase::DefineDamageConsiquences(const UDamageType* DamageType, float Damage)
{
	//TODO: Как убрать каст?
	const UAT_DamageTypeBase* IncomingDamageType = Cast<UAT_DamageTypeBase>(DamageType);

	if (IncomingDamageType->GetType() == EDamageType::Heal)
	{
		HealthComponent->Increase(CountReceivedHealing(Damage));
	}
	else
	{
		HealthComponent->Decrease(CountReceivedDamage(Damage, IncomingDamageType));
	}
}

void AATCharacterBase::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Решение получать, или нет.
	//if (bIsImmortal) return;
	
	// Если получать, то от кого.
	AATCharacterBase* Attacker = Cast<AATCharacterBase>(DamageCauser);
	ECharacterType AttakersType = Attacker->GetCharacterType();

	// Если персонаж не враг народа.
	if (CharacterType != ECharacterType::Enemy)
	{
		// И атака прилетела тоже не от врага народа, то не обращаем на неё внимания. Обознались, вадать.
		if (AttakersType != ECharacterType::Enemy)
		{
			return;
		}
	}
	// Если же ты враг народа.
	else
	{
		// То ишак ишака видит издалека...
		if (AttakersType == ECharacterType::Enemy)
		{
			return;
		}
	}
	// Если хочется поиграться с разными воюющими друг с другом непесями, можно и усложнить через switch.

	// CountDamage
	DefineDamageConsiquences(DamageType, Damage);
	// PlayMontage OnTakeDamage
	PlayOnHitMontage();
}

void AATCharacterBase::OnEndHealth()
{
	if (!bIsDead)
	{
		bIsDead = true;

		// Выбираем рандомную секцию  для проигрывания. Подразумевается, что анимакция помирания не одна и выбирается рандомно.
		int32 AnimCount = MontageOnDie->CompositeSections.Num() - 1;
		FName SectionName = MontageOnDie->GetAnimCompositeSection(FMath::RandRange(0, AnimCount)).SectionName;

		// Чтобы не дёргался, если получит по рылу во время смерти.
		if (IsValid(GetMesh()))
		{
			// Физика оставлена для последующего ragdoll`a. Хотя, наверное можно и потом включить.
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}

		DetachFromControllerPendingDestroy();

		// Заканчиваем все анимации
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance->IsAnyMontagePlaying())
		{
			float BlendTime = 0.2f;
			AnimInstance->StopAllMontages(BlendTime);
		}

		PlayAnimMontage(MontageOnDie, OnDiePlayRate, SectionName);
	}
}

// Called when the game starts or when spawned
void AATCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(PowerComponent)
	check(WeaponComponent);

	SetMovementBehaviour(MovementBehaviour);

	OnTakeAnyDamage.AddDynamic(this, &AATCharacterBase::OnTakeDamage);

	//SpawnWeapon();

	// Привязка делегатов.
	if (HealthComponent)
	{
		HealthComponent->OnEnd.AddDynamic(this, &AATCharacterBase::OnEndHealth);
	}
}

void AATCharacterBase::OnDied()
{
	if (IsValid(GetMesh()) && IsValid(GetCapsuleComponent()))
	{
		// А если выключить этот кривой Ragdoll?
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
	}
	// Дальше можно удалить чара через некоторое время.

}

float AATCharacterBase::CountReceivedDamage(float DamageAmount, const UAT_DamageTypeBase* DamageType)
{
	return DamageAmount;
}

float AATCharacterBase::CountReceivedHealing(float HealAmount)
{
	return HealAmount;
}

float AATCharacterBase::CalculateOutputDamage(const AATWeaponBase* Weapon)
{
	float DamageMultiplier = 1.f;
	
	return Weapon->GetBaseDamage() * DamageMultiplier;
}

bool AATCharacterBase::IsMakingAction()
{
	if (GetMesh()->GetAnimInstance())
	{
		return GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	}

	return false;
}

void AATCharacterBase::Jump()
{
	Super::Jump();
}

void AATCharacterBase::Dash()
{

}

void AATCharacterBase::Action()
{

}

void AATCharacterBase::MeleeAttack()
{
}

void AATCharacterBase::RangeAttack()
{
}

void AATCharacterBase::DealDamage(const FHitResult& HitResult, const AATWeaponBase* Weapon)
{
	
	AActor* DamagedActor = HitResult.GetActor();
	if (!IsValid(DamagedActor))
	{
		return;
	}

	float DamageAmount = CalculateOutputDamage(Weapon);
	// Здесь можно сделать фильтр по профилу коллизий и наносить урон только врагам, или герою, бочкам и т.д.
	// Но тогда придётся добавлять сюда кучу имён профилей...
	// А так решение о получении урона или всяком таком принимается уже в самом повреждаемом акторе, где реализована функция TakeDamage.
	// Решение принимается на базе класса наносящего урон актора... Это каст... Это дорого...
	UGameplayStatics::ApplyDamage(DamagedActor, DamageAmount, GetController(), this, Weapon->GetDamageType());
}

EMovementBehaviour AATCharacterBase::GetMovementBehaviour() const
{
	return MovementBehaviour;
}

ECharacterType AATCharacterBase::GetCharacterType() const
{
	return CharacterType;
}

void AATCharacterBase::SetMovementBehaviour(EMovementBehaviour Behaviour)
{
	switch (Behaviour)
	{
	case EMovementBehaviour::Crouch:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

		break;
	
	case EMovementBehaviour::Walk:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


		break;
	
	case EMovementBehaviour::Run:
		
		MovementBehaviour = Behaviour;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

		break;
	
	default:
		
		break;
	}
}

void AATCharacterBase::ClearAim()
{
	AimComponent->ClearAim();
}

//bool AATCharacterBase::IsInCombat() const
//{
//	return bIsInCombat;
//}
//
//void AATCharacterBase::SetCombatState(bool InCombat)
//{
//	bIsInCombat = InCombat;
//	//OnCombatting.Broadcast(bIsInCombat);
//}

AATWeaponBase* AATCharacterBase::GetMeleeWeapon() const
{
	return WeaponComponent->GetMeleeWeapon();
}

// Called every frame
void AATCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AATCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

