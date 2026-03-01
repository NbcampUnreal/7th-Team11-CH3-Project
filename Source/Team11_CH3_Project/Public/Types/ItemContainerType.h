// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemContainerType : uint8
{
	Inventory,
	Equipment,
	PartsSockets,
	SkillGem,
	Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotChanged, UItemSlot*, SlotData);
