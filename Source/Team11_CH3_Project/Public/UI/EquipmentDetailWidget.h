// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Items/ItemSlot.h"
#include "EquipmentDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentDetailWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init();
	void HandleItemSlotChanged(const UItemSlot* SlotData, int32 INT32);
};
