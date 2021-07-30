// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ATResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChange, float, CurrentResValue, float, MaxResValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADMINSTALE_API UATResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UATResourceComponent();

private:

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRecover = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RecoveryAmount = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RecoveryTickFrequency = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentAmount = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxAmount = 100.f;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable)
	FOnEnd OnEnd;

	UPROPERTY(BlueprintAssignable)
	FOnChange OnChange;
	
	UFUNCTION(BlueprintCallable)
	virtual void Decrease(float Value);

	UFUNCTION(BlueprintCallable)
	virtual void Increase(float Value);

	UFUNCTION(BlueprintCallable)
	float GetCurrentAmount();

	UFUNCTION(BlueprintCallable)
	float GetMaxAmount() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetMaxAmount(float Value);

	UFUNCTION(BlueprintCallable)
	bool IsRecover() const;

	UFUNCTION(BlueprintCallable)
	void EnableRecovery(bool Enable);

	UFUNCTION(BlueprintCallable)
	float GetRecoveryAmount() const;

	UFUNCTION(BlueprintCallable)
	void SetRecoveryAmount(float Value);

	UFUNCTION(BlueprintCallable)
	float GetRecoveryTickFrequency() const;

	UFUNCTION(BlueprintCallable)
	void SetRecoveryTickFrequency(float Value);

};
