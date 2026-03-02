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
	if (ItemDataAsset)
	{
		return ItemDataAsset->GetItemName();
	}
	return FText::GetEmpty();
}

EItemType UItemInstance::GetItemType() const
{
	if (ItemDataAsset){
		return ItemDataAsset->GetItemType();
	}
	return EItemType::Max;
}

UItemDataAsset* UItemInstance::GetItemDataAsset() const
{
	if (ItemDataAsset){
		return ItemDataAsset.Get();
	}
	return nullptr;
}


int32 UItemInstance::GetCount() const
{
	return Count;
}

void UItemInstance::AddCount(int32 InCount)
{
	if (!ItemDataAsset)
	{
		return;
	}
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
