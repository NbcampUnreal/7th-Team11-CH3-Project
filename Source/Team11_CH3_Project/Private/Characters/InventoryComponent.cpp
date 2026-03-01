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
	UItemInstance* ItemInstance = NewObject<UItemInstance>(this, ItemDataAsset->GetInstanceClass());
	ItemInstance->Init(ItemDataAsset, Count);
	if (ItemInstance->IsValid() == false)
	{
		return false;
	}
	EItemType ItemType = ItemInstance->GetItemType();
	int32 Index = -1;

	//중첩
	if (ItemType == EItemType::Material || ItemType == EItemType::Usable)
	{
		for (Index = 0; Index < InventorySlots.Num(); ++Index)
		{
			if (InventorySlots[Index]->ItemInstance && InventorySlots[Index]->ItemInstance->GetItemDataAsset()->
			                                                                  GetItemID() == ItemInstance->
				GetItemDataAsset()->GetItemID())
			{
				break;
			}
		}
		if (InventorySlots.IsValidIndex(Index))
		{
			ItemInstance->ConditionalBeginDestroy();
			InventorySlots[Index]->ItemInstance->AddCount(ItemInstance->GetCount());
			OnInventorySlotChanged.Broadcast(InventorySlots[Index], ContainerType, Index);
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
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], ContainerType, Index);
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
		InventorySlots[Index]->ItemInstance->Clear();
		InventorySlots[Index]->ItemInstance = nullptr;
	}
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], ContainerType, Index);
	return true;
}

UItemInstance* UInventoryComponent::GetItem(int32 TargetIndex)
{
	if (!InventorySlots.IsValidIndex(TargetIndex))
	{
		return nullptr;
	}
	return InventorySlots[TargetIndex]->ItemInstance;
}

bool UInventoryComponent::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	if (!InventorySlots.IsValidIndex(Index))
	{
		return false;
	}
	InventorySlots[Index]->ItemInstance = ItemInstance;
	InventorySlots[Index]->ItemType = ItemInstance->GetItemType();
	OnInventorySlotChanged.Broadcast(InventorySlots[Index], ContainerType, Index);
	return true;
}

bool UInventoryComponent::CanReceiveItem(UItemInstance* ItemInstance, int32 TargetIndex)
{
	if (!InventorySlots.IsValidIndex(TargetIndex))
	{
		return false;
	}
	return true;
}

bool UInventoryComponent::SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex)
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
