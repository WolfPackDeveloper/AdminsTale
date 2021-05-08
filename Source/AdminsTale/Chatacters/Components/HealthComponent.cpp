// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Engine/Engine.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
	Health = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//���. �������� ����� � ���������...
	AActor* HealthOwner = GetOwner();

	if (HealthOwner)
	{
		HealthOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.f)
	{
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

		// Debug
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Health is: %f"), Health));
	}
}

void UHealthComponent::RestoreHealth(float HealingAmount)
{
	if (HealingAmount > 0.f)
	{
		Health = FMath::Clamp(Health + HealingAmount, 0.f, MaxHealth);
	}
}
