// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropTableTypes.generated.h"

class UItemDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDropItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemDataAsset> ItemDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance = 0.f;
};

// 마스터 DataTable 각 행: 스테이지+난이도 → 서브 DataTable
USTRUCT(BlueprintType)
struct FDropTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StageIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Difficulty = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> DropTable;
};
