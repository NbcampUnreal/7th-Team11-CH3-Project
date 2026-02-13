// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/EquipmentItemData.h"
#include "ArmorItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  TEAM11_CH3_PROJECT_API FArmorItemData : public FEquipmentItemData
{
	GENERATED_BODY()
	// ArmorActor 생성 예정
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon")
	//TSoftClassPtr<class AArmorActor> AmorActorClass;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Armor")
	//EArmorSlot ArmorSlot;
};
