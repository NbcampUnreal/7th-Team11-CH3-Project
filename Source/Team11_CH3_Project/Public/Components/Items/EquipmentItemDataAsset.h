// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "Types/StatTypes.h"
#include "EquipmentItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:
	EEquipmentType GetEquipmentType()const{return EquipmentType;}
	TMap<EStat,float> GetStatBonuses() const{return StatBonuses;}
	int32 GetMaxPartsCount()const{return MaxPartsCount;}
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Equipment")
	int32 MaxPartsCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Equipment")
	EEquipmentType EquipmentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Equipment")
	TMap<EStat, float> StatBonuses;
};
