// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/EquipmentInstance.h"

#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/ItemSlot.h"

void UEquipmentInstance::Init(UEquipmentItemDataAsset* InItemDataAsset, int32 InMaxGemCount)
{
	Super::Init(InItemDataAsset, 1);
	UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(InItemDataAsset);
	if (EquipmentItemDataAsset)
	{
		EquipmentType = EquipmentItemDataAsset->GetEquipmentType();
	}
	Sockets.Empty();
	MaxGemCount = InMaxGemCount;
	Sockets.SetNum(MaxGemCount);
	for (int32 Index = 0; Index < Sockets.Num(); Index++)
	{
		UItemSlot* ItemSlot = NewObject<UItemSlot>(this);
		Sockets[Index] = ItemSlot;
		Sockets[Index]->ItemInstance = nullptr;
		Sockets[Index]->ItemType = EItemType::Parts;
	}
}

UItemInstance* UEquipmentInstance::GetItem(int32 Index)
{
	if (!Sockets.IsValidIndex(Index))
	{
		return nullptr;
	}
	return Sockets[Index]->ItemInstance;
}

bool UEquipmentInstance::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	if (!Sockets.IsValidIndex(Index))
	{
		return false;
	}
	Sockets[Index]->ItemInstance = ItemInstance;
	//TODO Recalculate Stat And BroadCast
	
	return true;
}

bool UEquipmentInstance::CanReceiveItem(UItemInstance* ItemInstance, int32 TargetIndex)
{
	if (ItemInstance->GetItemType() == EItemType::Parts)
	{
		return true;
	}
	return false;
}

bool UEquipmentInstance::SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex)
{
	if (OtherContainer == nullptr)
	{
		return false;
	}
	UItemInstance* OtherItemInstance = OtherContainer->GetItem(OtherIndex);
	UItemInstance* MyItemInstance = GetItem(MyIndex);
	if (!CanReceiveItem(OtherItemInstance, MyIndex))
	{
		return false;
	}
	if (!OtherContainer->CanReceiveItem(MyItemInstance, OtherIndex))
	{
		return false;
	}
	SetItemAt(OtherItemInstance, MyIndex);
	OtherContainer->SetItemAt(MyItemInstance, OtherIndex);
	return true;
}
