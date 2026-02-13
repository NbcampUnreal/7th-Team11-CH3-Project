// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatTypes.generated.h"

UENUM(BlueprintType)
enum class EStat : uint8
{
	MaxHP,
	DEF,
	MoveSpeed,
	AttackDamage,
	CastSpeed,
	ProjectileSpeed,
	CriticalChance,
	CriticalDamage,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Ranged
};