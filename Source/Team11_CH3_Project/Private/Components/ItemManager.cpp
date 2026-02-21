// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManager.h"
#include "Components/Skillmanager.h"
#include "Components/BuffManager.h"
#include "Components/Skills/BaseSkill.h"
#include "Components/StatComponent.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/ItemWorldSubsystem.h"
#include "WeaponActor.h"

// Sets default values for this component's properties
UItemManager::UItemManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UItemManager::UseItem(FName RowName, EItemType ItemType, int32 SlotIndex)
{
	UItemWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UItemWorldSubsystem>();
	if (IsValid(Subsystem) == false)
		return;

	switch (ItemType)
	{
	case EItemType::Potion:
		if (auto* Data = Subsystem->FindPotion(RowName))
		{
			UsePotion(Data);
		}
		break;
	case EItemType::SkillGem:
		if (auto* Data = Subsystem->FindSkillGem(RowName))
		{
			UseSkillGem(Data, SlotIndex);
		}
		break;
	case EItemType::Equipment:
		if (auto* Data = Subsystem->FindWeapon(RowName))
		{
			EquipWeapon(Data);
		}
		else if (auto* ArmorData = Subsystem->FindArmor(RowName))
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
		UE_LOG(LogTemp, Warning, TEXT("Heal : %0.f"), Data->HealAmount);
	}
}

void UItemManager::UseSkillGem(FSkillGemItemData* Data, int32 SlotIndex)
{
	auto* SkillManager = GetOwner()->FindComponentByClass<USkillManager>();
	
	if (IsValid(SkillManager) == false || Data->SkillData.IsNull())
		return;
	USkillDataAsset* LoadedSkill = Data->SkillData.LoadSynchronous();
	if (IsValid(LoadedSkill) == false)
		return;

	SkillManager->EquipSkillGem(SlotIndex, LoadedSkill);
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
		Weapon->Init(Data, Mesh);

		CurrentWeapon = Weapon;
		CachedWeaponData = *Data;
		ApplyStatBonuses(Data->ItemID, Data->StatBonuses);

		// PlayerCharacter의 WeaponActor 갱신
		if (auto* Player = Cast<APlayerCharacter>(GetOwner()))
		{
			Player->SetWeaponActor(Weapon);
		}
	}
}

void UItemManager::EquipArmor(FArmorItemData* Data)
{
	UnequipArmor(Data->EquipmentType);
	// 장착 중인 방어구 타입 및 데이터 추가
	EquippedArmors.Add(Data->EquipmentType, *Data);
	// StatBonuses 추가
	ApplyStatBonuses(Data->ItemID, Data->StatBonuses);
}

void UItemManager::ApplyStatBonuses(FName ItemRowName, TMap<EStat, float>& StatBonuses)
{
	auto* BuffManager = GetOwner()->FindComponentByClass<UBuffManager>();
	if (IsValid(BuffManager) == false)
		return;
	TArray<int32>& BuffIDS = EquipmentBuffIDs.FindOrAdd(ItemRowName);
	for (auto& Pair : StatBonuses)
	{
		int32 ID = BuffManager->AddBuff(Pair.Key, EBuffType::Add, Pair.Value, -1.f);
		BuffIDS.Add(ID);
	}
}

void UItemManager::RemoveStatBonuses(FName ItemID)
{
	UBuffManager* BuffManager = GetOwner()->FindComponentByClass<UBuffManager>();
	if (IsValid(BuffManager) == false)
		return;
	TArray<int32>* BuffIDs = EquipmentBuffIDs.Find(ItemID);
	if (BuffIDs == nullptr)
		return;

	for (int32 ID : *BuffIDs)
	{
		BuffManager->RemoveBuff(ID);
	}
	EquipmentBuffIDs.Remove(ItemID);
	
}

void UItemManager::UnequipWeapon()
{
	if (IsValid(CurrentWeapon) == false)
		return;

	RemoveStatBonuses(CachedWeaponData.ItemID);
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

void UItemManager::UnequipArmor(EEquipmentType SlotType)
{
	if (EquippedArmors.Contains(SlotType) == false)
		return;

	RemoveStatBonuses(EquippedArmors[SlotType].ItemID);
	EquippedArmors.Remove(SlotType);

}
