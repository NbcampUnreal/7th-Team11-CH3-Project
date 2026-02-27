#include "Characters/InventoryComponent.h"

#include "Components/Items/ItemSlot.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	InventorySlots.SetNum(InventorySize);
	for (int32 i = 0; i < InventorySize; i++)
	{
		UItemSlot* InventorySlot = NewObject<UItemSlot>(this);
		InventorySlots[i] = (InventorySlot);
	}
}

bool UInventoryComponent::AddItem(UItemInstance* ItemInstance, int32 Amount)
{
	if (!IsValid(ItemInstance))
	{
		return false;
	}
	EItemType ItemType = ItemInstance->GetItemType();
	int32 Index = -1;
	
	//중첩
	if (ItemType == EItemType::Material || ItemType == EItemType::Potion)
	{
		if (Indexes.Contains(ItemInstance->GetItemName()))
		{
			Index = Index = Indexes[ItemInstance->GetItemName()];
			ItemInstance->ConditionalBeginDestroy();
			InventorySlots[Index]->ItemInstance->AddCount(Amount);
			OnInventorySlotChanged.Broadcast(InventorySlots[Index], Index);
			return true;
		}
	}
	//단일
	for (Index = 0; Index < InventorySlots.Num(); Index++)
	{
		if (!IsValid(InventorySlots[Index]->ItemInstance) || InventorySlots[Index]->ItemInstance->IsValid() ==
			false)
		{
			break;
		}
	}
	if (!InventorySlots.IsValidIndex(Index))
	{
		return false;
	}

	InventorySlots[Index]->ItemInstance = ItemInstance;
	Indexes.Add(ItemInstance->GetItemName(), Index);
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], Index);
	return true;
}

bool UInventoryComponent::RemoveItem(int32 Index, int32 Amount)
{
	if (!InventorySlots.IsValidIndex(Index) || !IsValid(InventorySlots[Index]->ItemInstance) || InventorySlots[Index]->
		ItemInstance->IsValid() == false)
	{
		return false;
	}
	InventorySlots[Index]->ItemInstance->AddCount(-Amount);
	if (InventorySlots[Index]->ItemInstance->GetCount() <= 0)
	{
		FName ItemName = InventorySlots[Index]->ItemInstance->GetItemName();
		if (Indexes.Contains(ItemName) && Indexes[ItemName] == Index)
		{
			Indexes.Remove(ItemName);
		}
		InventorySlots[Index]->ItemInstance->Clear();
		InventorySlots[Index]->ItemInstance = nullptr;
	}
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], Index);
	return true;
}
