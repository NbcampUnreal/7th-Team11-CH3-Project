// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipmentDetailWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "UI/InteractableItemSlotWidget.h"
#include "UI/StatRowWidget.h"
#include "UI/MainInventoryWidget.h"

void UEquipmentDetailWidget::Init(UMainInventoryWidget* InMainInventoryWidget)
{
	//TODO
	//장비 이미지, 설명, 젬슬롯관리
	MainInventoryWidget = InMainInventoryWidget;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UEquipmentDetailWidget::Update(UItemSlot* ItemSlot)
{
	if (IsValid(ItemSlot) == false || IsValid(ItemSlot->GetItemInstance()) == false)
		return;
	UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemSlot->GetItemInstance());
	if (IsValid(EquipmentInstance) == false)
		return;
	CurrentEquipment = EquipmentInstance;
	UItemDataAsset* ItemData = EquipmentInstance->GetItemDataAsset();
	if (IsValid(ItemData) == false)
		return;

	// 이름, 설명
	if (ItemName)
	{
		ItemName->SetText(ItemData->GetItemName());
	}
	if (DESC_Text)
	{
		DESC_Text->SetText(ItemData->GetItemDesc());
	}
	// 썸네일
	if (IsValid(EquipmentThumbnail))
	{
		EquipmentThumbnail->Init(MainInventoryWidget.Get(), ItemSlot);
	}
	// 파츠 슬롯
	if (IsValid(ItemSlotWidgetClass) == false)
		return;

	GemSlots.Empty();
	if (GemBox)
	{
		GemBox->ClearChildren();
		const TArray<TObjectPtr<UItemSlot>>& Sockets = EquipmentInstance->GetPartsSlots();
		for (int32 i = 0; i < Sockets.Num(); i++)
		{
			UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, ItemSlotWidgetClass);
			GemBox->AddChildToHorizontalBox(NewSlot);
			GemSlots.Add(NewSlot);
			NewSlot->Init(MainInventoryWidget.Get(), Sockets[i]);
		}
	}
	UpdateStatBox(EquipmentInstance->GetCachedStats());
}

void UEquipmentDetailWidget::HandlePartsSlotChanged(UItemSlot* SlotData)
{
	if (!IsValid(SlotData))
	{
		return;
	}
	if (!CurrentEquipment.IsValid())
	{
		return;
	}
	if (GemSlots.IsValidIndex(SlotData->GetIndex()))
	{
		GemSlots[SlotData->GetIndex()]->UpdateSlot(SlotData);
	}
	UpdateStatBox(CurrentEquipment->GetCachedStats());
}

void UEquipmentDetailWidget::UpdateStatBox(const TMap<EStat, float>& Stats)
{
	StatRows.Empty();
	if (IsValid(StatBox) == false)
		return;
	StatBox->ClearChildren();
	if (IsValid(StatRowWidgetClass) == false)
		return;

	for (const auto& Pair : Stats)
	{
		UStatRowWidget* Row = CreateWidget<UStatRowWidget>(this, StatRowWidgetClass);
		if (IsValid(Row) == false)
			return;

		Row->Update(Pair.Key, Pair.Value);
		StatBox->AddChildToVerticalBox(Row);
		StatRows.Add(Row);
	}
}
