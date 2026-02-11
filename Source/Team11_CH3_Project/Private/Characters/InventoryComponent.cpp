#include "Characters/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::HasWeaponEquipped() const
{
	return false;
}