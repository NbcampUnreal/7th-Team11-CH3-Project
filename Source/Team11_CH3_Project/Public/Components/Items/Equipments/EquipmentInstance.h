// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "ItemInstance.h"
#include "Types/ItemTypes.h"
#include "Types/StatTypes.h"
#include "UObject/Object.h"
#include "EquipmentInstance.generated.h"

class UItemSlot;
class UPartsItemDataAsset;
class UEquipmentItemDataAsset;

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatsRecalculated, EEquipmentType, Type, UEquipmentInstance*, Instance);

UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentInstance : public UItemInstance, public IItemContainer
{
	GENERATED_BODY()

public:
	virtual void Init(UItemDataAsset* InItemDataAsset, int32 InCount) override;	
	virtual EItemContainerType GetItemContainerType() const override;
	virtual UItemInstance* GetItem(int32 Index) override;
	virtual bool SetItemAt(UItemInstance* ItemInstance, int32 Index) override;
	virtual bool CanReceiveItem(UItemInstance* ItemInstance, int32 TargetIndex) override;	
	virtual bool SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex) override;
	
	//void Init(UEquipmentItemDataAsset* InItemDataAsset, int32 InMaxGemCount=3);
	//void EquipParts(UItemInstance* PartsItemDataAsset, int32 Index);
	//void UnEquipParts(int32 Index);
	// Stat 계산 및 불러오기
	void CalculateStats();
	float GetStat(EStat Stat) const;
	const TArray<TObjectPtr<UItemSlot>>& GetPartsSlots() const { return Sockets; }
	EEquipmentType GetEquipmentType() const { return EquipmentType; }
	const TMap<EStat, float>& GetCachedStats() const { return CachedStats; }
	// 장비 장착된 상태 확인 및 변경(ItemManger에서)
	bool IsEquipped() const { return bIsEquipped; }
	void SetIsEquipped(bool bInIsEquipped) { bIsEquipped = bInIsEquipped; }

	// 브로드 캐스트 
	UPROPERTY(BlueprintAssignable)
	FOnStatsRecalculated OnStatsRecalculated;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UItemSlot>> Sockets;
	
private:
	int32 MaxGemCount = 3;
	EEquipmentType EquipmentType;
	// Container대신 EquipmentInstance에서 Stat보유
	UPROPERTY()
	TMap<EStat, float> CachedStats;
	// 지금 장착된 상태인지 아닌지
	bool bIsEquipped = false;
};
