// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemOverlayWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Items/ItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Items/Equipments/ItemInstance.h"
#include "Types/ItemTypes.h"
#include "UI/ItemSlotWidget.h"
#include "UI/MainInventoryWidget.h"
#include "UI/StatRowWidget.h"

void UItemOverlayWidget::Init(UMainInventoryWidget* InMainInventoryWidget)
{
	MainInventoryWidget = InMainInventoryWidget;
	if (Sockets.Num() > 0)
	{
		return;
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemName->SetText(FText::FromString(TEXT("NOITEM")));
	
	if (!ItemSlotWidgetClass)
	{
		return;
	}
	if (PartsBox && SocketBox)
	{
		SocketBox->ClearChildren();
		for (int32 i = 0; i < 3; i++)
		{
			UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this, ItemSlotWidgetClass);
			SocketBox->AddChildToHorizontalBox(NewSlot);
			Sockets.Add(NewSlot);
			NewSlot->UpdateSlot(nullptr);
		}
	}
	StatRows.Empty();
	StatBox->ClearChildren();
}

void UItemOverlayWidget::UpdateOverlayWidget(FVector2D ScreenPosition, UItemInstance* ItemInstance)
{
	SetVisibility(ESlateVisibility::Visible);
	
	if (UCanvasPanelSlot* CanvasPanelSlot =  Cast<UCanvasPanelSlot> (Slot))
	{
		FVector2D LocalPosition = MainInventoryWidget->GetCachedGeometry().AbsoluteToLocal(ScreenPosition);
		CanvasPanelSlot->SetPosition(LocalPosition + FVector2D(20.0f, 20.0f));
	}
	if (!StatBox || !SocketBox || !ItemInstance)
	{
		return;
	}
	if (ItemName)
	{
		ItemName->SetText(ItemInstance->GetItemName());
	}
	if (ItemType)
	{
		if (UEnum* EnumPtr = StaticEnum<EItemType>())
		{
			FText DisplayText = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(ItemInstance->GetItemType()));
			ItemType->SetText(DisplayText);
		}
	}
	if (DESC_Text)
	{
		DESC_Text->SetText(ItemInstance->GetItemDataAsset()->GetItemDesc());
	}
	switch (ItemInstance->GetItemType())
	{
	case EItemType::Equipment:
		if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance))
		{
			if (EquipmentInstance->GetEquipmentType() == EEquipmentType::SkillGem)
			{
				UpdateStatBox(EquipmentInstance);
				CollapseSocketBox();
			}else
			{
				UpdateStatBox(EquipmentInstance);
				UpdateSocketBox(EquipmentInstance);
			}
		}
		break;
	case EItemType::Material:
		CollapseStatBox();
		CollapseSocketBox();
		break;
	case EItemType::Usable:
		CollapseStatBox();
		CollapseSocketBox();
		break;
	case EItemType::Parts:
		if (UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance))
		{
			UpdateStatBox(EquipmentInstance);
		}else
		{
			CollapseStatBox();
		}
		CollapseSocketBox();
		break;
	default: ;
	}
}

void UItemOverlayWidget::ClearOverlayWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	StatRows.Empty();
	StatBox->ClearChildren();
}

void UItemOverlayWidget::UpdateStatBox(UEquipmentInstance* EquipmentInstance)
{
	StatBox->SetVisibility(ESlateVisibility::Visible);
	if (StatRowWidgetClass)
	{
		//TODO
	}
}

void UItemOverlayWidget::UpdateSocketBox(UEquipmentInstance* EquipmentInstance)
{
	SocketBox->SetVisibility(ESlateVisibility::Visible);
	TArray<TObjectPtr<UItemSlot>> PartsSlots = EquipmentInstance->GetPartsSlots();
	for (int i = 0; i < 3; i++)
	{
		if (Sockets.IsValidIndex(i) && PartsSlots.IsValidIndex(i))
		{
			Sockets[i]->UpdateSlot(PartsSlots[i]);
		}
	}
}

void UItemOverlayWidget::CollapseStatBox()
{
	StatBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemOverlayWidget::CollapseSocketBox()
{
	SocketBox->SetVisibility(ESlateVisibility::Collapsed);
	
}
