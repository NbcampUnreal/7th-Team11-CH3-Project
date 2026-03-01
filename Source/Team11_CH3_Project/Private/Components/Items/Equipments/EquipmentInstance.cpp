// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Items/PartsItemDataAsset.h"
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

void UEquipmentInstance::EquipParts(UItemInstance* GemItemDataAsset, int32 Index)
{
	if (Sockets.IsValidIndex(Index))
	{
		Sockets[Index]->ItemInstance = GemItemDataAsset;
		CalculateStats();
	}
}

void UEquipmentInstance::UnEquipParts(int32 Index)
{
	if (Sockets.IsValidIndex(Index))
	{
		Sockets[Index]->ItemInstance = nullptr;
		CalculateStats();
	}
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
		if (Slot->IsValid() == false)
			continue;
		UPartsItemDataAsset* Parts = Cast<UPartsItemDataAsset>(Slot->ItemInstance->GetItemDataAsset());
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
