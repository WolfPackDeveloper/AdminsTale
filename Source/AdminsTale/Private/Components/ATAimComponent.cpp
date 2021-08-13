// Fill out your copyright notice in the Description page of Project Settings.


#include "ATAimComponent.h"
#include "ATCharacterBase.h"

#include "Curves/CurveFloat.h" // Timeline curvefloat
#include "Engine/Engine.h" // Onscreen debug message
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h" //Trace
#include "Kismet/KismetMathLibrary.h" // Math, LookAt

// Sets default values for this component's properties
UATAimComponent::UATAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UATAimComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AATCharacterBase>(GetOwner());
}

void UATAimComponent::TakeAim()
{
	AimedActor = nullptr;
	TArray<TEnumAsByte<EObjectTypeQuery>> AimObjectTypes;
	AimObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);

	// В настройках проекта предварительно создаётся новый тип (будет шестым в массиве, после всяких так WorldStatic и прочей лабуды).
	// Первые шесть элементов создаются движком, остальное можно самому придумывать.
	//ObjectTypeQuery1 - world static
	//ObjectTypeQuery2 - world dynamic
	//ObjectTypeQuery3 - pawn
	//ObjectTypeQuery4 - physics body
	//ObjectTypeQuery5 - vehicle
	//ObjectTypeQuery6 - destructible
	//ObjectTypeQuery7 - custom 1

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FHitResult AimHitResult;
	FVector TraceStart = Character->GetActorLocation();
	FVector TraceEnd = TraceStart + Character->GetActorForwardVector() * AimRange;

	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		AimRadius,
		AimObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		AimHitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f
	);

	if (Hit)
	{
		AimedActor = AimHitResult.GetActor();
		// Проверка на обоснованность доводки. Если отклонение больше 30 градусов - целься лучше.
		float CurrentYaw = Character->GetActorRotation().Yaw;
		float AimYaw = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), AimedActor->GetActorLocation()).Yaw;
		
		//Debug
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentYaw: %f"), CurrentYaw));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("AimYaw: %f"), AimYaw));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DiffYaw: %f"), AimYaw - CurrentYaw));

		if (FMath::Abs(AimYaw - CurrentYaw) <= AimAngle)
		{
			if (IsValid(TurnCurveFloat))
			{
				this->SetComponentTickEnabled(true);
				
				FOnTimelineFloat TurningToAim;
				TurningToAim.BindUFunction(this, FName("TurnToAim"));
				TurnTimeline.AddInterpFloat(TurnCurveFloat, TurningToAim);
				TurnTimeline.SetLooping(false);
				TurnTimeline.PlayFromStart();
			}
		}
	}
}

AActor* UATAimComponent::GetAim() const
{
	return AimedActor;
}

void UATAimComponent::ClearAim()
{
	AimedActor = nullptr;
}

void UATAimComponent::TurnToAim(float CurveValue)
{
	float Yaw = FMath::Lerp(Character->GetActorRotation().Yaw, UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), AimedActor->GetActorLocation()).Yaw, CurveValue);

	FRotator ToAimRotation = FRotator(0.f, Yaw, 0.f);

	Character->SetActorRotation(ToAimRotation);
}

// Called every frame
void UATAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Debug
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("AimComponent tick")));

	// А что если включать тик компонента только на время удара?!
	if (AimedActor)
	{
		TurnTimeline.TickTimeline(DeltaTime);
	}
	else
	{
		this->SetComponentTickEnabled(false);
	}
}

