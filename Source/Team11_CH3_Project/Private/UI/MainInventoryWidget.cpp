// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainInventoryWidget.h"

#include "Components/Items/Equipments/EquipmentItemData.h"
#include "UI/EquipmentDetailWidget.h"
#include "UI/InventoryWidget.h"

void UMainInventoryWidget::Init(int32 InventorySize)
{
	InventoryWidget->Init(InventorySize);
	EquipmentDetailWidget->Init();
}

void UMainInventoryWidget::ToggleEquipmentDetailWidget()
{
}

void UMainInventoryWidget::HandleItemSlotChanged(const UItemSlot* SlotData, EItemContainerType ItemContainerType,
                                                 int32 SlotIndex)
{
	FEquipmentItemData* EquipmentItemData = nullptr;
	switch (ItemContainerType)
	{
	//장비와 인벤토리를 관리하는 위젯
	case EItemContainerType::Inventory:
		InventoryWidget->HandleInventoryItemSlotChanged(SlotData,SlotIndex);
		break;
	case EItemContainerType::Equipment:
		InventoryWidget->HandleInventoryItemSlotChanged(SlotData,SlotIndex);
		break;
	//젬 슬롯 업데이트
	case EItemContainerType::Parts:
		break;
	}
}
