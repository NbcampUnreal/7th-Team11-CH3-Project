// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainInventoryWidget.h"

#include "UI/EquipmentDetailWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemOverlayWidget.h"
#include "UI/StatDetailWidget.h"
#include "MainPlayerController.h"
#include "Characters/PlayerCharacter.h"

void UMainInventoryWidget::Init(int32 InventorySize, UInventoryComponent* InInventoryComponent,
	UItemManager* InEquipmentComponent)
{
	if (InventoryWidget)
		InventoryWidget->Init(this, InInventoryComponent,InEquipmentComponent);
	if (EquipmentDetailWidget)
		EquipmentDetailWidget->Init(this);
	if (ItemOverlayWidget)
		ItemOverlayWidget->Init(this);
	InventoryComponent = InInventoryComponent;
	EquipmentComponent = InEquipmentComponent;
	InventoryComponent->OnInventorySlotChanged.AddDynamic(this, &UMainInventoryWidget::HandleItemSlotChanged);
	EquipmentComponent->OnEquipmentSlotChanged.AddDynamic(this, &UMainInventoryWidget::HandleItemSlotChanged);
}

void UMainInventoryWidget::UpdateEquipmentDetailWidget(UItemSlot* EquipmentSlot)
{
	EquipmentDetailWidget->SetVisibility(ESlateVisibility::Visible);
	//TODO 	
	EquipmentDetailWidget->Update(EquipmentSlot);
	//EquipmentDetailWidget->UpdateOverlayWidget(UEquipmentInstance* ItemInstance);
}

void UMainInventoryWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	EquipmentDetailWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void UMainInventoryWidget::HandleItemSlotChanged(UItemSlot* SlotData)
{
	switch (SlotData->GetItemContainer()->GetItemContainerType())
	{
	//장비와 인벤토리를 관리하는 위젯
	case EItemContainerType::Inventory:
		InventoryWidget->HandleInventoryItemSlotChanged(SlotData);
		break;
	case EItemContainerType::Equipment:
		InventoryWidget->HandleEquipmentItemSlotChanged(SlotData);
		break;
	case EItemContainerType::PartsSockets:
		EquipmentDetailWidget->HandlePartsSlotChanged(SlotData);
		break;
	case EItemContainerType::Max:
		break;
	}
}

void UMainInventoryWidget::HandleStatChanged(UStatComponent* StatComp)
{
	if (StatDetailWidget)
	{
		TMap<EStat, float> StatMap;
		if (UEnum* EnumPtr = StaticEnum<EStat>())
		{
			for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
			{
				EStat Type = static_cast<EStat>(EnumPtr->GetValueByIndex(i));
				StatMap.Add(Type, StatComp->GetCurrentStat(Type));
			}
		}
		StatDetailWidget->UpdateStatBox(StatMap);
	}
}

FReply UMainInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab || InKeyEvent.GetKey() == EKeys::Escape)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwningPlayer());
		if (PC)
		{
			PC->HandleOpenInventory();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}