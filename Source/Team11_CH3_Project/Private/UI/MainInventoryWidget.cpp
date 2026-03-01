// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainInventoryWidget.h"

#include "UI/EquipmentDetailWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemOverlayWidget.h"

void UMainInventoryWidget::Init(int32 InventorySize, UInventoryComponent* InInventoryComponent,
	UItemManager* InEquipmentComponent)
{
	if (InventoryWidget)
		InventoryWidget->Init(this, InventorySize);
	if (EquipmentDetailWidget)
		EquipmentDetailWidget->Init(this);
	if (ItemOverlayWidget)
		ItemOverlayWidget->Init(this);
	InventoryComponent = InInventoryComponent;
	EquipmentComponent = InEquipmentComponent;
}

void UMainInventoryWidget::UpdateEquipmentDetailWidget(const UEquipmentInstance* EquipmentSlot)
{
	EquipmentDetailWidget->SetVisibility(ESlateVisibility::Visible);
	//TODO 	
	//EquipmentDetailWidget->UpdateOverlayWidget(UEquipmentInstance* ItemInstance);
}

void UMainInventoryWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	EquipmentDetailWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	case EItemContainerType::PartsSockets:
		if (const UEquipmentSlot* EquipmentSlot = Cast<UEquipmentSlot>(SlotData))
		{
			EquipmentDetailWidget->HandlePartsSlotChanged(EquipmentSlot, SlotIndex);
		}
		break;
	case EItemContainerType::SkillGem:
		//TODO
		InventoryWidget->HandleSkillGemItemSlotChanged(SlotData, SlotIndex);
		break;
	case EItemContainerType::Max:
		break;
	}
}
