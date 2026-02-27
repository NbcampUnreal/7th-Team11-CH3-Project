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
	Parts,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemSlotChanged, const UItemSlot*, SlotData, EItemContainerType, ItemContainerType, int32, SlotIndex);
