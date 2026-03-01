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
		SkillGemSlot1->Init(InMainInventoryWidget);
	}
	if (SkillGemSlot2)
	{
		SkillGemSlot2->Init(InMainInventoryWidget);
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
			NewSlot->Init(InMainInventoryWidget);
		}
	}
	HeadSlot->Init(InMainInventoryWidget);
	ChestSlot->Init(InMainInventoryWidget);
	LegsSlot->Init(InMainInventoryWidget);
	WeaponSlot->Init(InMainInventoryWidget);
	HandSlot->Init(InMainInventoryWidget);
	FeetSlot->Init(InMainInventoryWidget);
	

	
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
