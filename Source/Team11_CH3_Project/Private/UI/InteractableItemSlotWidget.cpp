// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractableItemSlotWidget.h"

#include "ItemDragDropOperation.h"
#include "Blueprint/DragDropOperation.h"
#include "Characters/InventoryComponent.h"
#include "Components/ItemManager.h"
#include "Components/Items/ItemSlot.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Items/Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/MainInventoryWidget.h"


void UInteractableItemSlotWidget::Init(UMainInventoryWidget* InMainInventoryWidget, UItemSlot* InSlot)
{
	Super::Init(InMainInventoryWidget, InSlot);
	bIsDraging = false;
}


FReply UInteractableItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDraging = false;
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInteractableItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsDraging && ItemSlot.IsValid())
	{	
		UItemInstance* ItemInstance = ItemSlot->GetItemInstance();
		if (ItemInstance && ItemInstance->GetItemType() == EItemType::Equipment)
		{
			if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance))
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
	if (!ItemSlot.IsValid())
	{
		return;
	}
	UItemInstance* ItemInstance = ItemSlot->GetItemInstance();
	if (!ItemInstance)
	{
		return;
	}
	
	bIsDraging = true;
	
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	DragOperation->OriginSlot = this;
	OutOperation = DragOperation;
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
}

bool UInteractableItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	bIsDraging = false;
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation || !DragOperation->OriginSlot.IsValid())
	{
		return false;
	}

	IItemContainer* Origin = DragOperation->OriginSlot->GetItemSlot()->GetItemContainer();
	int32 OriginIndex = DragOperation->OriginSlot->GetItemSlot()->GetIndex();

	IItemContainer* Destination = ItemSlot->GetItemContainer();
	int32 DestinationIndex = ItemSlot->GetIndex();

	return Origin->SwapItems(OriginIndex, Destination, DestinationIndex);
}

void UInteractableItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	bIsDraging = false;
}