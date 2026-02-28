// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/ItemInstance.h"

#include "Components/Items/ItemDataAsset.h"

void UItemInstance::Init(UItemDataAsset* InItemDataAsset, int32 InCount)
{
	ItemDataAsset = InItemDataAsset;
	Count = InCount;
}

FText UItemInstance::GetItemName() const
{
	return ItemDataAsset->GetItemName();
}

EItemType UItemInstance::GetItemType() const
{
	return ItemDataAsset->GetItemType();
}

UItemDataAsset* UItemInstance::GetItemDataAsset() const
{
	return ItemDataAsset.Get();
}


int32 UItemInstance::GetCount() const
{
	return Count;
}

void UItemInstance::AddCount(int32 InCount)
{
	if (ItemDataAsset->GetMaxStackCount() < Count+InCount)
	{
		return;
	}
	if (Count+InCount < 0)
	{
		return;
	}
	Count += InCount;
}

bool UItemInstance::IsValid() const
{
	if (Count == 0) return false;
	if (!ItemDataAsset) return false;
	return true;
}

void UItemInstance::Clear()
{
	ItemDataAsset = nullptr;
	Count = 0;
}
