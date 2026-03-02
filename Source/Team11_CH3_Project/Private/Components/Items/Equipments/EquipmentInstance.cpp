// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Items/PartsItemDataAsset.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/ItemSlot.h"

void UEquipmentInstance::Init(UItemDataAsset* InItemDataAsset, int32 InCount)
{
	Super::Init(InItemDataAsset, 1);
	if (UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(InItemDataAsset))
	{
		EquipmentType = EquipmentItemDataAsset->GetEquipmentType();
		MaxGemCount = EquipmentItemDataAsset->GetMaxPartsCount();
	}
	Sockets.Empty();
	Sockets.SetNum(MaxGemCount);
	for (int32 Index = 0; Index < Sockets.Num(); Index++)
	{
		UItemSlot* ItemSlot = NewObject<UItemSlot>(this);
		ItemSlot->Init(this, Index);
		Sockets[Index] = ItemSlot;
	}
}

EItemContainerType UEquipmentInstance::GetItemContainerType() const
{
	return EItemContainerType::PartsSockets;
}

UItemInstance* UEquipmentInstance::GetItem(int32 Index)
{
	if (!Sockets.IsValidIndex(Index))
	{
		return nullptr;
	}
	return Sockets[Index]->GetItemInstance();
}

bool UEquipmentInstance::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	if (!Sockets.IsValidIndex(Index))
	{
		return false;
	}
	Sockets[Index]->SetItemInstance(ItemInstance);
	
	CalculateStats();
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

void UEquipmentInstance::CalculateStats()
{
	CachedStats.Empty();
	UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(ItemDataAsset);
	if (EquipmentItemDataAsset == nullptr)
		return;
	for (auto& Pair : EquipmentItemDataAsset->GetStatBonuses())
	{
		CachedStats.FindOrAdd(Pair.Key) += Pair.Value;
	}

	for (UItemSlot* Slot : Sockets)
	{
		if (Slot == nullptr)
			continue;
		UPartsItemDataAsset* Parts = Cast<UPartsItemDataAsset>(Slot->GetItemInstance()->GetItemDataAsset());
		if (Parts == nullptr)
			continue;

		for (auto& Pair : Parts->GetStatBonuses())
		{
			CachedStats.FindOrAdd(Pair.Key) += Pair.Value;
		}

	}

	if (bIsEquipped == false)
		return;

	OnStatsRecalculated.Broadcast(EquipmentType, this);
}

float UEquipmentInstance::GetStat(EStat Stat) const
{
	const float* Found = CachedStats.Find(Stat);
	return Found ? *Found : 0.f;
}
