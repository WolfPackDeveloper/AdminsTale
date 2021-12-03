// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ATWidgetComponent.generated.h"


class AATCharacterBase;
//class UATNameplateWidget;
class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADMINSTALE_API UATWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UATWidgetComponent();

private:

	UPROPERTY()
	AATCharacterBase* Character = nullptr;

	//UPROPERTY()
	//UATNameplateWidget* Nameplate = nullptr;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* Nameplate = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	//UWidgetComponent* CombatText = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Nameplate")
	//UATNameplateWidget* Nameplate = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Nameplate")
	//TSubclassOf<UATNameplateWidget> NameplateWidgetClass;
	
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
