// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManager.h"
#include "Components/Skillmanager.h"
#include "Components/BuffManager.h"
#include "Components/StatComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/ItemWorldSubsystem.h"
#include "WeaponActor.h"
#include "Characters/Monster/MonsterBase.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/GemItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "Components/Items/WeaponItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"

// Sets default values for this component's properties
UItemManager::UItemManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UItemManager::InitializeComponent()
{
	Super::InitializeComponent();
	UEnum* EnumPtr = StaticEnum<EEquipmentType>();
	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(EnumPtr->GetValueByIndex(i));
			UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);
			EquipmentSlot->ItemType = EItemType::Equipment;
			EquipmentSlots.Add(Type, EquipmentSlot);
		}
	}
	GemSlots.SetNum(MaxSKillGemCount);
	for (int32 i = 0; i < MaxSKillGemCount; ++i)
	{
		UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);

		GemSlots[i] = EquipmentSlot;
		GemSlots[i]->ItemType = EItemType::Equipment;
		GemSlots[i]->EquipmentType = EEquipmentType::SkillGem;
		GemSlots[i]->ItemInstance = nullptr;
	}
}

void UItemManager::Equip(UEquipmentInstance* Data)
{
	//TODO STAT
	if (UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(Data->GetItemDataAsset()))
	{
		if (EEquipmentType::Weapon == EquipmentItemDataAsset->GetEquipmentType())
		{
			EquipWeapon(Data);
		}
	}
}

void UItemManager::Unequip(EEquipmentType SlotType)
{
	//TODO STAT
}


TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> UItemManager::GetEquipments()
{
	TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> Ret;
	for (auto& Pair : EquipmentSlots)
	{
		Ret.Add(Pair.Key, Cast<UEquipmentInstance>(Pair.Value->ItemInstance));
	}
	return Ret;
}

void UItemManager::Clear()
{
	Super::InitializeComponent();
	UEnum* EnumPtr = StaticEnum<EEquipmentType>();
	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(EnumPtr->GetValueByIndex(i));
			Unequip(EEquipmentType::Weapon);
		}
	}
}

// Called when the game starts
void UItemManager::BeginPlay()
{
	Super::BeginPlay();


	// ...
}


void UItemManager::EquipWeapon(UEquipmentInstance* WeaponItemInstance)
{
	UnequipWeapon();
	UWeaponItemDataAsset* WeaponItemDataAsset = Cast<UWeaponItemDataAsset>(WeaponItemInstance->GetItemDataAsset());
	if (!WeaponItemDataAsset)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();


	AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(
		WeaponItemDataAsset->GetWeaponActorClass().LoadSynchronous(), SpawnParams);
	if (IsValid(Weapon))
	{
		auto* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		Weapon->Init(WeaponItemDataAsset, Mesh);

		CurrentWeapon = Weapon;
		if (EquipmentSlots.Find(EEquipmentType::Weapon))
		{
			EquipmentSlots[EEquipmentType::Weapon]->ItemInstance = WeaponItemInstance;
			EquipmentSlots[EEquipmentType::Weapon]->EquipmentType = EEquipmentType::Weapon;
		}
		// PlayerCharacter의 WeaponActor 갱신
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
		{
			if (USkillManager* SkillComponent = Player->FindComponentByClass<USkillManager>())
			{
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
		}
		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner()))
		{
			if (USkillManager* SkillComponent = Monster->FindComponentByClass<USkillManager>())
			{
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
		}
	}
}

void UItemManager::EquipSkillGem(UEquipmentInstance* SkillGemInstance, int32 Index)
{
	if (!GemSlots.IsValidIndex(Index))
	{
		return;
	}

	GemSlots[Index]->ItemInstance = SkillGemInstance;
	if (AActor* Owner = GetOwner())
	{
		if (USkillManager* SkillComponent = Owner->FindComponentByClass<USkillManager>())
		{
			if (UGemItemDataAsset* GemItemDataAsset = Cast<UGemItemDataAsset>(SkillGemInstance->GetItemDataAsset()))
			{
				SkillComponent->EquipSkillGem(Index + 1, GemItemDataAsset->GetSkillData());
			}
		}
	}
}

void UItemManager::UnEquipSillGem(int32 Index)
{
	if (!GemSlots.IsValidIndex(Index))
	{
		return;
	}
	
	GemSlots[Index]->ItemInstance = nullptr;
	if (AActor* Owner = GetOwner())
	{
		if (USkillManager* SkillComponent = Owner->FindComponentByClass<USkillManager>())
		{
			SkillComponent->UnEquipSkillGem(Index+1);
		}
	}
}

void UItemManager::UnequipWeapon()
{
	if (IsValid(CurrentWeapon) == false)
		return;

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}


void UItemManager::RestoreEquipment(TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> EquipmentData)
{
	for (auto& Pair : EquipmentData)
	{
		Equip(Pair.Value);
	}
}
