// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "AdminsTale/Chatacters/CharacterBase.h"
#include "AdminsTale/Objects/DamageTypes/DT_Base.h"
#include "AdminsTale/Objects/DamageTypes/DT_Physical.h"
#include "AdminsTale/Objects/DamageTypes/DT_Healing.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	float CapsuleRadius = 10.f;
	float CapsuleHalfHeight = 50.f;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(Root);
	CapsuleComponent->SetCapsuleRadius(CapsuleRadius);
	CapsuleComponent->SetCapsuleHalfHeight(CapsuleHalfHeight);
	//����� ������� �� �������� � ����, � �� �� ����� � ������ ��-�� �����!!!
	//� ��������� � ����������� ������ ��� ���� ������.
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(CapsuleComponent);
	//������ ����� ������������� ������ � ��������� � �������.
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	bStrikes = false;

	MinDamage = 10;
	MaxDamage = 10;

	SetDamageEdge(TEXT("TipSocket"), TEXT("HandleSocket"));

	//SocketEdgeTop = TEXT("TipSocket");
	//SocketEdgeBottom = TEXT("HandleSocket");

	// ������������ � ������.
	DamageType = UDT_Physical::StaticClass();
}

float AWeapon::CalculateDamage(float DamageMultiplier)
{
	float BaseDamage = FMath::FRandRange(MinDamage, MaxDamage);
	
	return BaseDamage * DamageMultiplier;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStrikes)
	{
		DealDamage();
	}

}

UStaticMeshComponent* AWeapon::GetMesh() const
{
	if (IsValid(Mesh))
	{
		return Mesh;
	}

	return nullptr;
}

UCapsuleComponent* AWeapon::GetCapsuleComponent() const
{
	if (IsValid(CapsuleComponent))
	{
		return CapsuleComponent;
	}

	return nullptr;
}

void AWeapon::SetDamageEdge(FName EdgeTopSocket, FName EdgeBotomSocket)
{
	SocketEdgeTop = EdgeTopSocket;
	SocketEdgeBottom = EdgeBotomSocket;
}

void AWeapon::DealDamage()
{
	bStrikes = true;

	// ��� ��� ���� ������������ - ��� ����� ������ ������...
	// ������ - � ��� ��� ������ � �� ����� ������� ��������������?
	// ��, ����, ���-�� ���.
	// SocketEdgeTop
	//FName SocketTip = TEXT("TipSocket");
	// SocketEdgeBottom
	//FName SocketHandle = TEXT("HandleSocket");

	APawn* WeaponOwner = Cast<APawn>(GetOwner());
	if (WeaponOwner == nullptr) return;
	AController* OwnerController = WeaponOwner->GetController();
	if (OwnerController == nullptr) return;

	// ��� ��� ��� ������������...
	if (Mesh->DoesSocketExist(SocketEdgeBottom) && Mesh->DoesSocketExist(SocketEdgeTop))
	{
		FVector TraceStart = Mesh->GetSocketLocation(SocketEdgeTop);
		FVector TraceEnd = Mesh->GetSocketLocation(SocketEdgeBottom);
		float TraceRadius = CapsuleComponent->GetUnscaledCapsuleRadius();

		TArray<AActor*> ActorsToIgnore;
		// ������� ��������� ������.
		ActorsToIgnore.Add(GetOwner());
		// � �� ������ ������ ���� ������.
		ActorsToIgnore.Add(this);

		FHitResult DamageTarget;

		// C ������� �����������
		//const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		//	GetWorld(),
		//	TraceStart,
		//	TraceEnd,
		//	TraceRadius,
		//	UEngineTypes::ConvertToTraceType(ECC_Visibility),
		//	false,
		//	ActorsToIgnore,
		//	EDrawDebugTrace::ForDuration,
		//	DamageTarget,
		//	true,
		//	FLinearColor::Red,
		//	FLinearColor::Green,
		//	2.0f
		//);

		const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			TraceStart,
			TraceEnd,
			TraceRadius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			DamageTarget,
			true
		);

		if (bHit)
		{
			// ��� ������� ��������� ����� - �� ����, ����� ������ ���� ApplyDamage...
			// � ���� �� ������������ � DamageType. ���� �� ������� ��� ������ ������� - ��� ���� � �����.
			if (DamageType)
			{
				float Damage = CalculateDamage(Cast<ACharacterBase>(WeaponOwner)->CalculateDamageMultiplier());
				UGameplayStatics::ApplyDamage(DamageTarget.GetActor(), Damage, OwnerController, this, DamageType);
			}
			
			bStrikes = false;
		}
	}
}

void AWeapon::StopDamage()
{
	bStrikes = false;
}

