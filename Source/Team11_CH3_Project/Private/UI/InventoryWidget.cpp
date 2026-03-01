// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "UI/InteractableItemSlotWidget.h"
#include "UI/MainInventoryWidget.h"

void UInventoryWidget::Init(UMainInventoryWidget* InMainInventoryWidget, int32 InventorySize)
{
	MainInventoryWidget = InMainInventoryWidget;
	if (Inventory.Num() > 0)
	{
		return;
	}
	if (SkillGemSlot1)
	{
		SkillGemSlot1->Init(InMainInventoryWidget, 1, EItemContainerType::SkillGem);
	}
	if (SkillGemSlot2)
	{
		SkillGemSlot2->Init(InMainInventoryWidget, 2, EItemContainerType::SkillGem);
	}
	if (InventoryGrid)
	{
		InventoryGrid->ClearChildren();
		for (int32 i = 0; i < InventorySize; i++)
		{
			UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, ItemSlotWidgetClass);
			
			int32 Row = i / 5;
			int32 Column = i % 5;
			InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
			Inventory.Add(NewSlot);
			NewSlot->Init(InMainInventoryWidget, i, EItemContainerType::Inventory);
		}
	}
	HeadSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Helmet),EItemContainerType::Equipment,EEquipmentType::Helmet);
	ChestSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Chest),EItemContainerType::Equipment,EEquipmentType::Chest);
	LegsSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Legs),EItemContainerType::Equipment,EEquipmentType::Legs);
	WeaponSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Weapon),EItemContainerType::Equipment,EEquipmentType::Weapon);
	HandSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Gloves),EItemContainerType::Equipment,EEquipmentType::Gloves);
	FeetSlot->Init(InMainInventoryWidget,static_cast<int64> (EEquipmentType::Boots),EItemContainerType::Equipment,EEquipmentType::Boots);
	

	
}


void UInventoryWidget::HandleInventoryItemSlotChanged(const UItemSlot* SlotData, int32 SlotIndex)
{
	if (Inventory.IsValidIndex(SlotIndex))
	{
		Inventory[SlotIndex]->UpdateSlot(SlotData);
	}
}

void UInventoryWidget::HandleEquipmentItemSlotChanged(const UItemSlot* SlotData, int32 SlotIndex)
{
	//TODO Optimization
	UEquipmentItemDataAsset*  EquipmentItemData = Cast<UEquipmentItemDataAsset> (SlotData->ItemInstance->GetItemDataAsset());
	
	if (EquipmentItemData)
	{
		switch (EquipmentItemData->GetEquipmentType())
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
		case EEquipmentType::SkillGem:
			break;
		}
	}
}

void UInventoryWidget::HandleSkillGemItemSlotChanged(const UItemSlot* ItemSlot, int32 Index)
{
	UInteractableItemSlotWidget* TargetSlot = nullptr;
	if (Index == 1)
	{
		TargetSlot = SkillGemSlot1;
	}else if (Index == 2)
	{
		TargetSlot = SkillGemSlot2;
	}
	TargetSlot->UpdateSlot(ItemSlot);
}
