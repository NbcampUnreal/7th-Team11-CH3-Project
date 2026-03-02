// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "UI/InteractableItemSlotWidget.h"
#include "UI/MainInventoryWidget.h"

void UInventoryWidget::Init(UMainInventoryWidget* InMainInventoryWidget, UInventoryComponent* InventoryComponent, UItemManager* EquipmentComponent)
{
	MainInventoryWidget = InMainInventoryWidget;
	if (InventoryWidgets.Num() > 0)
	{
		return;
	}
	if (EquipmentComponent)
	{
		if (SkillGemBox && SkillGemWidgetClass)
		{
			SkillGemBox->ClearChildren();

			TArray<TObjectPtr<UEquipmentSlot>>& SkillGemSlots = EquipmentComponent->GetSkillGemSlots();
			for (int32 i = 0; i < SkillGemSlots.Num(); i++)
			{
				UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, SkillGemWidgetClass);
				SkillGemBox->AddChildToHorizontalBox(NewSlot);
				NewSlot->Init(InMainInventoryWidget, SkillGemSlots[i]);
				SKillGemWidgets.Add(NewSlot);
			}
			
		}
		

		HeadSlot->Init(InMainInventoryWidget, EquipmentComponent->GetEquipmentSlot(EEquipmentType::Helmet));
		ChestSlot->Init(InMainInventoryWidget,EquipmentComponent->GetEquipmentSlot(EEquipmentType::Chest));
		LegsSlot->Init(InMainInventoryWidget,EquipmentComponent->GetEquipmentSlot(EEquipmentType::Legs));
		WeaponSlot->Init(InMainInventoryWidget,EquipmentComponent->GetEquipmentSlot(EEquipmentType::Weapon));
		HandSlot->Init(InMainInventoryWidget,EquipmentComponent->GetEquipmentSlot(EEquipmentType::Gloves));
		FeetSlot->Init(InMainInventoryWidget,EquipmentComponent->GetEquipmentSlot(EEquipmentType::Boots));
	}
	if (InventoryComponent)
	{
		if (InventoryGrid)
		{
			InventoryGrid->ClearChildren();
			TArray<TObjectPtr<UItemSlot>> InventorySlots = InventoryComponent->GetInventorySlots();
			for (int32 i = 0; i < InventorySlots.Num(); i++)
			{
				UInteractableItemSlotWidget* NewSlot = CreateWidget<UInteractableItemSlotWidget>(this, ItemSlotWidgetClass);
			
				int32 Row = i / 5;
				int32 Column = i % 5;
				InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
				NewSlot->Init(InMainInventoryWidget, InventorySlots[i]);
				InventoryWidgets.Add(NewSlot);
			}
		}
	}
}


void UInventoryWidget::HandleInventoryItemSlotChanged(UItemSlot* SlotData)
{
	if (InventoryWidgets.IsValidIndex(SlotData->GetIndex()))
	{
		InventoryWidgets[SlotData->GetIndex()]->UpdateSlot(SlotData);
	}
}

void UInventoryWidget::HandleEquipmentItemSlotChanged(UItemSlot* SlotData)
{
	//TODO Optimization -> 아이템 타입을 슬롯에서 가져오도록 교체
	UEquipmentSlot* EquipSlot = Cast<UEquipmentSlot>(SlotData);
	if (IsValid(EquipSlot) == false)
		return;
	switch (EquipSlot->GetEquipmentType())
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

void UInventoryWidget::HandleSkillGemItemSlotChanged(UItemSlot* ItemSlot)
{
	int32 Index = ItemSlot->GetIndex();
	if (SKillGemWidgets.IsValidIndex(Index))
	{
		SKillGemWidgets[Index]->UpdateSlot(ItemSlot);
	}
}
