// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTexture2D* GetThumbnail() const { return Thumbnail.LoadSynchronous(); }
	FName GetItemID() const { return ItemID; }
	FText GetItemName() const { return ItemName; }
	FText GetItemDesc() const { return Desc; }
	EItemType GetItemType() const { return ItemType; }
	UClass* GetInstanceClass() const { return InstanceClass; }
	EItemRarity GetRarity() const { return Rarity; }
	int32 GetMaxStackCount() const { return MaxStackCount; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<UItemInstance> InstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity Rarity;
	// 인벤토리 중첩 소지 갯수(Potion 및 Material 빼고는 1로 통일)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackCount = 1;
};
