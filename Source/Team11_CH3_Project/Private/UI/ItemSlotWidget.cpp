// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Items/ItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "UI/ItemOverlayWidget.h"
#include "UI/MainInventoryWidget.h"



void UItemSlotWidget::Init(UMainInventoryWidget* InMainInventoryWidget, int32 InIndex,
	EItemContainerType InItemContainerType, EEquipmentType InEquipmentType)
{
	MainInventoryWidget = InMainInventoryWidget;
	UpdateSlot(nullptr);
	ItemContainerType = InItemContainerType;
	EquipmentType = InEquipmentType;
	Index = InIndex;
}

void UItemSlotWidget::UpdateSlot(const UItemSlot* InSlot)
{

	if (!IsValid(InSlot) )
	{
		Clear();
		return;
	}
	if (!InSlot->ItemInstance || !InSlot->ItemInstance->IsValid())
	{
		Clear();
		return;
	}
	ItemInstance = InSlot->ItemInstance;
	if (UItemDataAsset* ItemData = InSlot->ItemInstance->GetItemDataAsset())
	{
		if (Thumbnail)
		{
			Thumbnail->SetBrushFromTexture(ItemData->GetThumbnail());
			Thumbnail->SetVisibility(ESlateVisibility::Visible);
		}
		if (Count)
		{
			Count->SetText(FText::AsNumber(ItemInstance->GetCount()));
			Count->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		Clear();
	}
}

void UItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (MainInventoryWidget.IsValid())
	{
		UItemOverlayWidget* ItemOverlayWidget = MainInventoryWidget->GetItemOverlayWidget();
		if (ItemOverlayWidget && ItemInstance.IsValid())
		{
			FVector2D ScreenPosition = InMouseEvent.GetScreenSpacePosition();
			ItemOverlayWidget->UpdateOverlayWidget(ScreenPosition, ItemInstance.Get());
		}

	}
	
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (MainInventoryWidget.IsValid())
	{
		if (UItemOverlayWidget* ItemOverlayWidget = MainInventoryWidget->GetItemOverlayWidget())
		{
			ItemOverlayWidget->ClearOverlayWidget();
		}
	}
}


void UItemSlotWidget::Clear()
{
	if (Thumbnail)
	{
		Thumbnail->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (Count){ 
		Count->SetVisibility(ESlateVisibility::Collapsed);
	}
	ItemInstance = nullptr;
}
