// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

enum class EItemType : uint8;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	void Init(FDataTableRowHandle InHandle, int32 InCount);
	FName GetItemName() const;
	EItemType GetItemType() const;
	FDataTableRowHandle GetItemDataHandle() const;
	int32 GetCount() const;
	void AddCount(int32 InCount);
	bool IsValid() const;
	void Clear();

protected:
	EItemType ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemDataHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Count = 0;
};
