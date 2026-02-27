// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/ItemInstance.h"

#include "Components/Items/ItemDataBase.h"
#include "Subsystems/ItemWorldSubsystem.h"

void UItemInstance::Init(FDataTableRowHandle InHandle, int32 InCount)
{
	ItemDataHandle = InHandle;
	Count = InCount;
	//TODo Item Subsystem
	UItemWorldSubsystem* ItemWorldSubsystem = GetWorld()->GetSubsystem<UItemWorldSubsystem>();
	if (ItemWorldSubsystem == nullptr)
	{
		return;
	}

	FItemDataBase* ItemData = ItemWorldSubsystem->FindItem(ItemDataHandle);
	ItemType = ItemData->ItemType;
}

FName UItemInstance::GetItemName() const
{
	return ItemDataHandle.RowName;
}

EItemType UItemInstance::GetItemType() const
{
	return ItemType;
}

FDataTableRowHandle UItemInstance::GetItemDataHandle() const
{
	return ItemDataHandle;
}

int32 UItemInstance::GetCount() const
{
	return Count;
}

void UItemInstance::AddCount(int32 InCount)
{
	Count += InCount;
}

bool UItemInstance::IsValid() const
{
	if (Count == 0) return false;
	if (ItemDataHandle.IsNull()) return false;
	return true;
}

void UItemInstance::Clear()
{
	ItemDataHandle = FDataTableRowHandle();
	Count = 0;
}
