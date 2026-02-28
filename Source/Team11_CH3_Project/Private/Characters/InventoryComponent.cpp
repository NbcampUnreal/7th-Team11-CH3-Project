#include "Characters/InventoryComponent.h"

#include "Components/Items/ItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "Subsystems/ItemWorldSubsystem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
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

bool UInventoryComponent::AddItem(UItemDataAsset* ItemDataAsset, int32 Count)
{
	if (!IsValid(ItemDataAsset))
	{
		return false;
	}
	UItemWorldSubsystem* ItemWorldSubsystem = GetWorld()->GetSubsystem<UItemWorldSubsystem>();
	UItemInstance* ItemInstance = NewObject<UItemInstance>(this);
	ItemInstance->Init(ItemDataAsset, Count);
	EItemType ItemType = ItemInstance->GetItemType();
	int32 Index = -1;
	
	//중첩
	if (ItemType == EItemType::Material || ItemType == EItemType::Potion)
	{
		if (Indexes.Contains(ItemDataAsset->GetItemID()))
		{
			Index = Index = Indexes[ItemDataAsset->GetItemID()];
			ItemInstance->ConditionalBeginDestroy();
			InventorySlots[Index]->ItemInstance->AddCount(ItemInstance->GetCount());
			OnInventorySlotChanged.Broadcast(InventorySlots[Index], EItemContainerType::Inventory, Index);
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
	Indexes.Add(ItemDataAsset->GetItemID(), Index);
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], EItemContainerType::Inventory, Index);
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
		FName ItemName = InventorySlots[Index]->ItemInstance->GetItemDataAsset()->GetItemID();
		if (Indexes.Contains(ItemName) && Indexes[ItemName] == Index)
		{
			Indexes.Remove(ItemName);
		}
		InventorySlots[Index]->ItemInstance->Clear();
		InventorySlots[Index]->ItemInstance = nullptr;
	}
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], EItemContainerType::Inventory, Index);
	return true;
}
