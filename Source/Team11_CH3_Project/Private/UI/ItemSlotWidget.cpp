// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Items/ItemSlot.h"
#include "Subsystems/ItemWorldSubsystem.h"

void UItemSlotWidget::UpdateSlot(const UItemSlot* InSlot)
{
	if (!IsValid(InSlot) )
	{
		Clear();
		return;
	}
	if ( !InSlot->ItemInstance->IsValid())
	{
		Clear();
		return;
	}
	if (UItemWorldSubsystem* ItemWorldSubsystem = GetWorld()->GetSubsystem<UItemWorldSubsystem>())
	{
		if (FItemDataBase* ItemData = ItemWorldSubsystem->FindItem(InSlot->ItemInstance->GetItemDataHandle()))
		{
			if (Thumbnail)
			{
				Thumbnail->SetBrushFromTexture(ItemData->Thumbnail.LoadSynchronous());
				Thumbnail->SetVisibility(ESlateVisibility::Visible);
			}
			if (Count)
			{
				Count->SetText(FText::AsNumber(InSlot->ItemInstance->GetCount()));
				Count->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			Clear();
		}
	}
	
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	//TODO 드래그중이 아니라면 디테일창 표시 현재는 장비만
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
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