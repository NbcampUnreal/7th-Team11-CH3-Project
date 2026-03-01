// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/EquipmentInstance.h"

#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/ItemSlot.h"

void UEquipmentInstance::Init(UEquipmentItemDataAsset* InItemDataAsset, int32 InMaxGemCount)
{
	Super::Init(InItemDataAsset, 1);
	UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(InItemDataAsset);
	if (!EquipmentItemDataAsset)
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

void UEquipmentInstance::EquipGem(UItemInstance* GemItemDataAsset, int32 Index)
{
	if (Sockets.IsValidIndex(Index))
	{
		Sockets[Index]->ItemInstance = GemItemDataAsset;
	}
	//TODO Recalculate Stat
}

void UEquipmentInstance::UnEquipGem(int32 Index)
{
	if (Sockets.IsValidIndex(Index))
	{
		Sockets[Index] = nullptr;
	}
	//TODO Recalculate Stat
}
