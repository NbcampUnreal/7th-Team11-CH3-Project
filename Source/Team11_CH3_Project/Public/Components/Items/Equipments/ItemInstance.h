// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDataAsset;
enum class EItemType : uint8;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TEAM11_CH3_PROJECT_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UItemDataAsset* InItemDataAsset, int32 InCount);
	FText GetItemName() const;
	EItemType GetItemType() const;
	UItemDataAsset* GetItemDataAsset() const;
	int32 GetCount() const;
	void AddCount(int32 InCount);
	bool IsValid() const;
	void Clear();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> ItemDataAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Count = 0;
};
