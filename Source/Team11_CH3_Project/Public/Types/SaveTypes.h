// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/ItemDataAsset.h"
#include "SaveTypes.generated.h"

/**
 * 
 */
 // 장비 저장 구조체(장비 본체 + 파츠 슬롯)
USTRUCT(BlueprintType)
struct FSavedEquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemDataAsset> DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UItemDataAsset>> PartSlots;
};

// 인벤토리 아이템 저장 구조체
USTRUCT(BlueprintType)
struct FSavedItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemDataAsset> DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;
};