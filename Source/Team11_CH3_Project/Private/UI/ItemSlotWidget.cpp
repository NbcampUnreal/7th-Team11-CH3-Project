// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Items/ItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "Subsystems/ItemWorldSubsystem.h"

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
	;
	if (UItemDataAsset* ItemData = InSlot->ItemInstance->GetItemDataAsset())
	{
		if (Thumbnail)
		{
			Thumbnail->SetBrushFromTexture(ItemData->GetThumbnail());
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