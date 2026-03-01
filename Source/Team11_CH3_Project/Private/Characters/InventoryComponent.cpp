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
		InventorySlot->Init(this, i);
		InventorySlots[i] = InventorySlot;
	}
}

bool UInventoryComponent::AddItem(UItemDataAsset* ItemDataAsset, int32 Count)
{
	if (!IsValid(ItemDataAsset))
	{
		return false;
	}
	UItemInstance* ItemInstance = NewObject<UItemInstance>(this, ItemDataAsset->GetInstanceClass());
	if (ItemDataAsset->GetInstanceClass() == UItemInstance::StaticClass())
	{
		ItemInstance->Init(ItemDataAsset, Count);
	}else if (ItemDataAsset->GetInstanceClass() == UEquipmentInstance::StaticClass())
	{
		UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance);
		EquipmentInstance->Init(ItemDataAsset, 3);
	}
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
			if (InventorySlots[Index]->GetItemInstance() && InventorySlots[Index]->GetItemInstance()->GetItemDataAsset()->
			                                                                  GetItemID() == ItemInstance->
				GetItemDataAsset()->GetItemID())
			{
				break;
			}
		}
		if (InventorySlots.IsValidIndex(Index))
		{
			ItemInstance->ConditionalBeginDestroy();
			InventorySlots[Index]->GetItemInstance()->AddCount(ItemInstance->GetCount());
			OnInventorySlotChanged.Broadcast(InventorySlots[Index]);
			return true;
		}
	}
	//단일
	for (Index = 0; Index < InventorySlots.Num(); Index++)
	{
		if (!IsValid(InventorySlots[Index]->GetItemInstance()) || InventorySlots[Index]->GetItemInstance()->IsValid() ==
			false)
		{
			break;
		}
	}
	if (!InventorySlots.IsValidIndex(Index))
	{
		return false;
	}

	InventorySlots[Index]->SetItemInstance(ItemInstance);
	OnInventorySlotChanged.Broadcast(InventorySlots[Index]);
	return true;
}

bool UInventoryComponent::RemoveItem(int32 Index, int32 Amount)
{
	if (!InventorySlots.IsValidIndex(Index) || !IsValid(InventorySlots[Index]->GetItemInstance()) ||
		InventorySlots[Index]->GetItemInstance()->IsValid() == false)
	{
		return false;
	}
	InventorySlots[Index]->GetItemInstance()->AddCount(-Amount);
	if (InventorySlots[Index]->GetItemInstance()->GetCount() <= 0)
	{
		InventorySlots[Index]->GetItemInstance()->Clear();
		InventorySlots[Index]->SetItemInstance(nullptr);
	}
	OnInventorySlotChanged.Broadcast(InventorySlots[Index]);
	return true;
}

EItemContainerType UInventoryComponent::GetItemContainerType() const
{
	return ContainerType;
}

UItemInstance* UInventoryComponent::GetItem(int32 TargetIndex)
{
	if (!InventorySlots.IsValidIndex(TargetIndex))
	{
		return nullptr;
	}
	return InventorySlots[TargetIndex]->GetItemInstance();
}

bool UInventoryComponent::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	if (!InventorySlots.IsValidIndex(Index))
	{
		return false;
	}
	InventorySlots[Index]->SetItemInstance(ItemInstance);
	OnInventorySlotChanged.Broadcast(InventorySlots[Index]);
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
