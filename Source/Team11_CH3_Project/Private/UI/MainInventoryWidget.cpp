// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainInventoryWidget.h"

#include "UI/EquipmentDetailWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemOverlayWidget.h"

void UMainInventoryWidget::Init(int32 InventorySize)
{
	if (InventoryWidget)
		InventoryWidget->Init(this, InventorySize);
	if (EquipmentDetailWidget)
		EquipmentDetailWidget->Init(this);
	if (ItemOverlayWidget)
		ItemOverlayWidget->Init(this);
}

void UMainInventoryWidget::ToggleEquipmentDetailWidget()
{
}

void UMainInventoryWidget::HandleItemSlotChanged(const UItemSlot* SlotData, EItemContainerType ItemContainerType,
                                                 int32 SlotIndex)
{
	switch (ItemContainerType)
	{
	//장비와 인벤토리를 관리하는 위젯
	case EItemContainerType::Inventory:
		InventoryWidget->HandleInventoryItemSlotChanged(SlotData, SlotIndex);
		break;
	case EItemContainerType::Equipment:
		InventoryWidget->HandleEquipmentItemSlotChanged(SlotData, SlotIndex);
		break;
	case EItemContainerType::Parts:
		if (const UEquipmentSlot* EquipmentSlot = Cast<UEquipmentSlot>(SlotData))
		{
			EquipmentDetailWidget->HandlePartsSlotChanged(EquipmentSlot, SlotIndex);
		}
		break;
	}
}
