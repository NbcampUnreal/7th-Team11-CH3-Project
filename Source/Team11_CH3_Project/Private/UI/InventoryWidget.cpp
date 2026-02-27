// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/Items/ItemSlot.h"
#include "Subsystems/ItemWorldSubsystem.h"
#include "Components/Items/ItemDataBase.h"
#include "UI/ItemSlotWidget.h"

void UInventoryWidget::Init(int32 InventorySize)
{
	Super::NativeConstruct();
	if (Inventory.Num() > 0)
	{
		return;
	}

	if (InventoryGrid)
	{
		InventoryGrid->ClearChildren();
		for (int32 i = 0; i < InventorySize; i++)
		{
			UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this, ItemSlotWidgetClass);
			int32 Row = i / 5;
			int32 Column = i % 5;
			InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
			Inventory.Add(NewSlot);
			NewSlot->UpdateSlot(nullptr);
		}
	}
}


void UInventoryWidget::HandleInventoryItemSlotChanged(const UItemSlot* SlotData, int32 SlotIndex)
{
	if (Inventory.IsValidIndex(SlotIndex))
	{
		Inventory[SlotIndex]->UpdateSlot(SlotData);
	}
}

void UInventoryWidget::HandleEquipmentItemSlotChanged(const UItemSlot* SlotData, EItemContainerType ItemContainerType, int32 SlotIndex)
{
	//TODO Optimization
	FEquipmentItemData* EquipmentItemData = GetWorld()->GetSubsystem<UItemWorldSubsystem>()->FindEquipment(
		SlotData->ItemInstance->GetItemDataHandle());
	if (EquipmentItemData)
	{
		switch (EquipmentItemData->EquipmentType)
		{
		case EEquipmentType::Weapon:
			WeaponSlot->UpdateSlot(SlotData);
			break;
		case EEquipmentType::Helmet:
			HeadSlot->UpdateSlot(SlotData);
			break;
		case EEquipmentType::Chest:
			ChestSlot->UpdateSlot(SlotData);
			break;
		case EEquipmentType::Gloves:
			HandSlot->UpdateSlot(SlotData);
			break;
		case EEquipmentType::Legs:
			LegsSlot->UpdateSlot(SlotData);
			break;
		case EEquipmentType::Boots:
			FeetSlot->UpdateSlot(SlotData);
			break;
		}
	}
}
