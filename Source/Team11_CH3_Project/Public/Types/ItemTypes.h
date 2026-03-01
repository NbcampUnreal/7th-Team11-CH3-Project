// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Potion,
    Equipment,
    Material,
    Usable
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
    Weapon,
    Helmet,
    Chest,
    Gloves,
    Legs,
    Boots,
    SkillGem
};