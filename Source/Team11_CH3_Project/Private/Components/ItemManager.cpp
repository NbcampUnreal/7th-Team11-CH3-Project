// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManager.h"
#include "Components/Skillmanager.h"
#include "Components/Skills/BaseSkill.h"
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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(
		Data->WeaponActorClass.LoadSynchronous(), SpawnParams);
	if (IsValid(Weapon))
	{
		auto* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		Weapon->Init(*Data, Mesh);

		CurrentWeapon = Weapon;
		CachedWeaponData = *Data;
		ApplyStatBonuses(Data->StatBonuses, false);
	}

}

void UItemManager::EquipArmor(FArmorItemData* Data)
{
	UnequipArmor(Data->EquipmentType);
	// 장착 중인 방어구 타입 및 데이터 추가
	EquippedArmors.Add(Data->EquipmentType, *Data);
	// StatBonuses 추가
	ApplyStatBonuses(Data->StatBonuses, false);
}

void UItemManager::ApplyStatBonuses(TMap<EStat, float> StatBonuses, bool bRemove)
{
	// Owner의 스탯 컴포넌트
	auto* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (!IsValid(StatComp))
		return;
	// StatBonuses Tmap 배열 순회 하면서 CurrntStat에 StatBonuses 적용
	for (auto& Pair : StatBonuses)
	{
		float Current = StatComp->GetCurrentStat(Pair.Key);
		float NewValue;
		// 장착 해제
		if (bRemove)
		{
			NewValue = Current - Pair.Value;
		}
		// 장착
		else
		{
			NewValue = Current + Pair.Value;
		}
		
		StatComp->SetCurrentStat(Pair.Key, NewValue);
	}
}

void UItemManager::UnequipWeapon()
{
	if (IsValid(CurrentWeapon) == false)
	{
		return;
	}

	ApplyStatBonuses(CachedWeaponData.StatBonuses, true);
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

void UItemManager::UnequipArmor(EEquipmentType SlotType)
{
	if (EquippedArmors.Contains(SlotType))
	{
		ApplyStatBonuses(EquippedArmors[SlotType].StatBonuses, true);
		EquippedArmors.Remove(SlotType);
	}
}
