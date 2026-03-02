// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipmentDetailWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "UI/InteractableItemSlotWidget.h"
#include "UI/MainInventoryWidget.h"

void UEquipmentDetailWidget::Init(UMainInventoryWidget* InMainInventoryWidget)
{
	//TODO
	//장비 이미지, 설명, 젬슬롯관리
	MainInventoryWidget = InMainInventoryWidget;

}

void UEquipmentDetailWidget::Update(UItemSlot* ItemSlot)
{
	// if (GemSlots.Num() > 0)
	// {
	// 	return;
	// }
	//
	// if (EquipmentThumbnail)
	// {
	// 	EquipmentThumbnail->Init(InMainInventoryWidget, 0, EItemContainerType::Max);
	// }
	//
	// if (!ItemSlotWidgetClass)
	// {
	// 	return;
	// }
	// if (GemBox)
	// {
	// 	GemBox->ClearChildren();
	// 	for (int32 i = 0; i < 3; i++)
	// 	{
	// 		UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, ItemSlotWidgetClass);
	// 		GemBox->AddChildToHorizontalBox(NewSlot);
	// 		GemSlots.Add(NewSlot);
	// 		NewSlot->Init(InMainInventoryWidget, i, EItemContainerType::PartsSockets);
	// 	}
	// }
	// StatRows.Empty();
	// StatBox->ClearChildren();
}

void UEquipmentDetailWidget::HandlePartsSlotChanged(UEquipmentSlot* SlotData)
{
	if (!IsValid(SlotData))
	{
		return;
	}
	if (SlotData->GetItemType() != EItemType::Equipment )
	{
		return;
	}
	if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(SlotData->GetItemInstance()))
	{
		const TArray<TObjectPtr<UItemSlot>>& Parts = EquipmentInstance->GetPartsSlots();
		if (Parts.IsValidIndex(SlotData->GetIndex()))
		{
			GemSlots[SlotData->GetIndex()]->UpdateSlot(SlotData);
		}
	}
}
