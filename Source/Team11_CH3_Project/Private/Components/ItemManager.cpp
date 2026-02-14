// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BaseSkill.h"
#include "Components/ItemManager.h"
#include "Components/Skillmanager.h"
#include "Components/StatComponent.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "WeaponActor.h"

// Sets default values for this component's properties
UItemManager::UItemManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UItemManager::UseItem(FName RowName, EItemType ItemType, int32 SlotIndex = -1)
{
	switch (ItemType)
	{
	case EItemType::Potion:
		if (auto* Data = PotionTable->FindRow<FPotionItemData>(RowName, TEXT("UItemManager::UseItem : Potion")))
		{
			UsePotion(Data);
		}
		break;
	case EItemType::SkillGem:
		if (auto* Data = SkillGemTable->FindRow<FSkillGemItemData>(RowName, TEXT("UItemManager::UseItem : SkillGem")))
		{
			UseSkillGem(Data, SlotIndex);
		}
		break;
	case EItemType::Equipment:
		if (auto* Data = WeaponTable->FindRow<FWeaponItemData>(RowName, TEXT("UItemManager::UseItem : Weapon")))
		{
			EquipWeapon(Data);
		}
		else if (auto* ArmorData = ArmorTable->FindRow<FArmorItemData>(RowName, TEXT("UItemManager::UseItem : Armor")))
		{
			EquipArmor(ArmorData);
		}
		break;
	//case EItemType::Material:
	//	break;
	default:
		break;
	}
}

// Called when the game starts
void UItemManager::BeginPlay()
{
	Super::BeginPlay();

	
	// ...
	
}

void UItemManager::UsePotion(FPotionItemData* Data)
{
	auto* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();

	if (IsValid(StatComp) && Data->HealAmount)
	{
		StatComp->AddCurrentHP(Data->HealAmount);
	}
}

void UItemManager::UseSkillGem(FSkillGemItemData* Data, int32 SlotIndex)
{
	auto* SkillManager = GetOwner()->FindComponentByClass<USkillManager>();
	
	if (IsValid(SkillManager) && Data->SkillClass)
	{
		SkillManager->EquipSkillGem(SlotIndex, Data->SkillClass);
	}

}

void UItemManager::EquipWeapon(FWeaponItemData* Data)
{
	UnequipWeapon();

}

void UItemManager::EquipArmor(FArmorItemData* Data)
{
}

void UItemManager::ApplyStatBonuses(TMap<EStat, float> StatBonuses, bool bRemove)
{
	auto* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (!IsValid(StatComp))
		return;

	for (auto& Pair : StatBonuses)
	{
		float Current = StatComp->GetCurrentStat(Pair.Key);
		float NewValue;
		if (bRemove)
		{
			NewValue = Current - Pair.Value;
		}
		else
		{
			NewValue = Current + Pair.Value;
		}
		
		StatComp->SetCurrentStat(Pair.Key, NewValue);
	}
}

void UItemManager::UnequipWeapon()
{
	if (CurrentWeapon)
	{
		ApplyStatBonuses(CachedWeaponData.StatBonuses, true);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void UItemManager::UnequipArmor(EEquipmentType SlotType)
{
	if (EquippedArmors.Contains(SlotType))
	{
		ApplyStatBonuses(EquippedArmors[SlotType].StatBonuses, true);
		EquippedArmors.Remove(SlotType);
	}
}
