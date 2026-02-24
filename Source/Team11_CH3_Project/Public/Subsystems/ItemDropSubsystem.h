// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DataTable.h"
#include "ItemDropSubsystem.generated.h"

/**
 * GameInstance에서 스테이지에 따라서 변동 -> 그러면 스테이지선택 따라 난이도가 바뀌는건?
 * 쉬운 난이도 선택하면 난이도 + 1 어려운 난이도 선택하면 난이도 + 2
 * 1  2  3  4  5 그럼 각 인덱스, 난이도로 현재 몇스테이지의 어떤 난이도인지 체크
 * 난이도는 포탈 선택할때 쉬움인지 어려움인지 골라서 넘겨주도록
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemDropSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void TryDropItem(FVector DropLocation);

private:
	TMap<TPair<int32, int32>, UDataTable*> DropTables;
};
