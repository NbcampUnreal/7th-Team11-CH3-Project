// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "Types/StatTypes.h"
#include "PartsItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UPartsItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:
	TMap<EStat, float> GetStatBonuses() const { return StatBonuses; }
protected:
	UPROPERTY(EditAnywhere, Category = "Item|Parts")
	TMap<EStat, float> StatBonuses;
};
