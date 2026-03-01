// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipmentDetailWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "UI/InteractableItemSlotWidget.h"

void UEquipmentDetailWidget::Init()
{
	//TODO
	//장비 이미지, 설명, 젬슬롯관리
	
	if (GemSlots.Num() > 0)
	{
		return;
	}

	if (GemBox)
	{
		GemBox->ClearChildren();
		for (int32 i = 0; i < 3; i++)
		{
			UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, ItemSlotWidgetClass);
			int32 Row = i / 5;
			int32 Column = i % 5;
			GemBox->AddChildToHorizontalBox(NewSlot);
			GemSlots.Add(NewSlot);
			NewSlot->UpdateSlot(nullptr);
		}
	}
}

void UEquipmentDetailWidget::HandlePartsSlotChanged(const UEquipmentSlot* SlotData, int32 Index)
{
	if (!IsValid(SlotData))
	{
		return;
	}
	if (SlotData->ItemType != EItemType::Equipment )
	{
		return;
	}
	if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(SlotData->ItemInstance))
	{
		const TArray<TObjectPtr<UPartsItemDataAsset>>& Parts = EquipmentInstance->GetPartsSlots();
		if (Parts.IsValidIndex(Index))
		{
			GemSlots[Index]->UpdateSlot(SlotData);
		}
	}
}
