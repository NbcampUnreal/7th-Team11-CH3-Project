// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "ItemInstance.h"
#include "Types/ItemTypes.h"
#include "UObject/Object.h"
#include "EquipmentInstance.generated.h"

class UItemSlot;
class UPartsItemDataAsset;
class UEquipmentItemDataAsset;
/**
 * 
 */
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
	
	const TArray<TObjectPtr<UItemSlot>>& GetPartsSlots() const { return Sockets; }
	EEquipmentType GetEquipmentType() const { return EquipmentType; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UItemSlot>> Sockets;
	
private:
	int32 MaxGemCount = 3;
	EEquipmentType EquipmentType;
	//TODO StatContainer
};
