// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/StatTypes.h"
#include "Components/Items/ItemDataBase.h"
#include "EquipmentItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  TEAM11_CH3_PROJECT_API FEquipmentItemData : public FItemDataBase
{
	GENERATED_BODY()
	
	// 장착 시 적용할 스탯 보정 WeaponItemData의 Damage도 여기에 통합가능(의논 필요)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Equipment")
	TMap<EStat, float> StatBonuses;
};
