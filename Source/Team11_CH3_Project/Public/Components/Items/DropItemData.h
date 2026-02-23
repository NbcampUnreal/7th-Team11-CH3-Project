// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/ItemDataBase.h"
#include "Components/Items/PickupActor.h"
#include "DropItemData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FDropItemData : public FItemDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Drop")
	float DropChance = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Drop")
	TSoftClassPtr<APickupActor> PickupActorClass;
};