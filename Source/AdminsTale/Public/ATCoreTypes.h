#pragma once
#include "ATCoreTypes.generated.h"

// CHARACTER

// Дерьмо для определения френдли фаера и буцкания бочек...
UENUM()
enum class ECharacterType : uint8
{
	Neutral,
	Player,
	Enemy,
	Ally
};

UENUM()
enum class EMovementBehaviour : uint8
{
	Run,
	Walk,
	Crouch
};

// DAMAGE TYPES

UENUM()
enum class EDamageType : uint8
{
	Physic,
	Fire,
	Heal
};

// WEAPON

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Axe,
	Hammer,
	Spear,
	Staff,
	Sword
};
