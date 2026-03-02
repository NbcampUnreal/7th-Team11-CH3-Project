// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Items/ItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "UI/ItemOverlayWidget.h"
#include "UI/MainInventoryWidget.h"




void UItemSlotWidget::Init(UMainInventoryWidget* InMainInventoryWidget, UItemSlot* InSlot)
{
	MainInventoryWidget = InMainInventoryWidget;
	UpdateSlot(InSlot);
}

void UItemSlotWidget::UpdateSlot(UItemSlot* InSlot)
{
	ItemSlot = InSlot;

	if (!IsValid(InSlot) )
	{
		Clear();
		return;
	}
	UItemInstance* ItemInstance = ItemSlot->GetItemInstance();
	if (!ItemInstance || !ItemInstance->IsValid())
	{
		Clear();
		return;
	}
	
	if (UItemDataAsset* ItemData = ItemInstance->GetItemDataAsset())
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
	if (UWidgetBlueprintLibrary::IsDragDropping())
	{
		return;
	}
	if (MainInventoryWidget.IsValid())
	{
		UItemOverlayWidget* ItemOverlayWidget = MainInventoryWidget->GetItemOverlayWidget();
		if (ItemOverlayWidget && ItemSlot.IsValid() )
		{
			if (UItemInstance* ItemInstance = ItemSlot->GetItemInstance()){
				FVector2D ScreenPosition = InMouseEvent.GetScreenSpacePosition();
				ItemOverlayWidget->UpdateOverlayWidget(ScreenPosition, ItemInstance);
			}
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
}
