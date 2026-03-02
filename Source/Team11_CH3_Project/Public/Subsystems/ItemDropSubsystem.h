// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DataTable.h"
#include "ItemDropSubsystem.generated.h"

/**
 *
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemDropSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void TryDropItem(FVector DropLocation);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> MasterDropTable;
	// 스테이지, 난이도 == 키, 서브 DataTable
	TMap<TPair<int32, int32>, TObjectPtr<UDataTable>> CachedDropTables;
};
