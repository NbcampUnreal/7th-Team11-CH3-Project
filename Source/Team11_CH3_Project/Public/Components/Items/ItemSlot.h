// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "Equipments/EquipmentInstance.h"
#include "Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "UObject/Object.h"
#include "ItemSlot.generated.h"
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemSlot : public UObject
{
	GENERATED_BODY()

public:
	void Init(IItemContainer* InItemContainer, int32 InIndex)
	{
		ItemContainer = Cast<UObject>(InItemContainer);
		Index = InIndex;
	}

	virtual void SetItemInstance(UItemInstance* InItemInstance) { ItemInstance = InItemInstance; }

	EItemType GetItemType() const
	{
		if (ItemInstance == nullptr)
		{
			return EItemType::Max;
		}
		return ItemInstance->GetItemType();
	}

	UItemInstance* GetItemInstance() { return ItemInstance; }
	IItemContainer* GetItemContainer() const { return Cast<IItemContainer>(ItemContainer.Get()); }
	int32 GetIndex() const { return Index; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemInstance> ItemInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<UObject> ItemContainer;
	int32 Index;
};

UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentSlot : public UItemSlot
{
	GENERATED_BODY()

public:
	void Init(IItemContainer* InItemContainer, int32 InIndex, EEquipmentType InEquipmentType)
	{
		Super::Init(InItemContainer, InIndex);
		EquipmentType = InEquipmentType;
	}

	virtual void SetItemInstance(UItemInstance* InItemInstance) override
	{
		Super::SetItemInstance(InItemInstance);
		if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance))
		{
			EquipmentType = EquipmentInstance->GetEquipmentType();
		}
	}

	UEquipmentInstance* GetEquipmentInstance() const { return Cast<UEquipmentInstance>(ItemInstance); }

	EEquipmentType GetEquipmentType() const { return EquipmentType; }

protected:
	EEquipmentType EquipmentType = EEquipmentType::Max;
};
