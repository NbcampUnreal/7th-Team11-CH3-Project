// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "UI/ItemSlotWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Inventory.Num() > 0)
	{
		return;
	}
	
	if (InventoryGrid)
	{
		InventoryGrid->ClearChildren();
		// 슬롯 동적 생성 (예: 컴포넌트의 슬롯 개수만큼)
		int32 SlotCount = 25;
        
		for (int32 i = 0; i < SlotCount; i++)
		{
			UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this,ItemSlotWidgetClass);
			int32 Row = i / 5; 
			int32 Column = i % 5;
			InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
			Inventory.Add(NewSlot);
			NewSlot->UpdateSlot(nullptr);
		}
	}
}



void UInventoryWidget::HandleItemSlotChanged(const UItemSlot* SlotData, EItemContainerType ItemContainerType,
                                             int32 SlotIndex)
{
	switch (ItemContainerType) {
	case EItemContainerType::Inventory:
		Inventory[SlotIndex]->UpdateSlot(SlotData);
		break;
	case EItemContainerType::Equipment:
		break;
	case EItemContainerType::Parts:
		break;
	}	
}

void UInventoryWidget::ToggleEquipmentDetailWidget()
{
}
