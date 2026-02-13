// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/ItemDataBase.h"
#include "PotionItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FPotionItemData : public FItemDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Potion")
	float HealAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Itme|Potion")
	float Duration = 0.f;
};
