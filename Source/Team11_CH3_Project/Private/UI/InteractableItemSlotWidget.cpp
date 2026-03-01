// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractableItemSlotWidget.h"

#include "ItemDragDropOperation.h"
#include "Blueprint/DragDropOperation.h"
#include "Characters/InventoryComponent.h"
#include "Components/ItemManager.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Items/Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "UI/MainInventoryWidget.h"


void UInteractableItemSlotWidget::Init(UMainInventoryWidget* InMainInventoryWidget, int32 InIndex,
	EItemContainerType InItemContainerType, EEquipmentType InEquipmentType)
{
	Super::Init(InMainInventoryWidget, InIndex, InItemContainerType, InEquipmentType);
	bIsDraging = false;
}

FReply UInteractableItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDraging = false;
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInteractableItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsDraging && ItemInstance.IsValid())
	{	
		if (ItemInstance->GetItemType() == EItemType::Equipment)
		{
			UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance.Get());
			if (EquipmentInstance)
			{
				MainInventoryWidget->UpdateEquipmentDetailWidget(EquipmentInstance);
			}
		}
	}
	//TODO 드래그중이 아니라면 디테일창 표시 현재는 장비만
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UInteractableItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (!ItemInstance.IsValid())
	{
		return;
	}
	bIsDraging = true;
	UItemDragDropOperation* DragOperation = NewObject<UItemDragDropOperation>(OutOperation);
	DragOperation->ItemInstance = ItemInstance.Get();
	DragOperation->Index = Index;
	DragOperation->ItemContainerType = ItemContainerType;
	OutOperation = DragOperation;
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
}

bool UInteractableItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
		
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation || !DragOperation->ItemInstance)
	{
		return false;
	}
	IItemContainer* Origin = nullptr;
	IItemContainer* Destination = nullptr;
	switch (DragOperation->ItemContainerType)
	{
	case EItemContainerType::Inventory:
		Origin = MainInventoryWidget->GetInventoryComponent();
		break;
	case EItemContainerType::Equipment:
		Origin = MainInventoryWidget->GetEquipmentComponent();
		break;
	case EItemContainerType::PartsSockets:
		
		break;
	case EItemContainerType::SkillGem:
		break;
	default: ;
	}
	
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInteractableItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}