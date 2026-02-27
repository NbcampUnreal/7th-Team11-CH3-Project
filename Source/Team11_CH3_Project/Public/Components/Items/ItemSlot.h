// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "UObject/Object.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemSlot : public UObject
{
	GENERATED_BODY()
public:
	EItemType ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemInstance> ItemInstance;
	bool IsValid() const { return ItemInstance->IsValid(); }
};

UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentSlot : public UItemSlot
{
	GENERATED_BODY()
public:
	EEquipmentType EquipmentType;
};
