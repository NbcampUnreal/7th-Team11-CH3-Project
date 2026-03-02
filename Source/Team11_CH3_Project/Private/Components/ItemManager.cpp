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
	bWantsInitializeComponent = true;

	// ...
}

void UItemManager::InitializeComponent()
{
	Super::InitializeComponent();
	if (UEnum* EnumPtr = StaticEnum<EEquipmentType>())
	{
		for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(EnumPtr->GetValueByIndex(i));
			UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);
			EquipmentSlot->Init(this, i, Type);
			EquipmentSlots.Add(EquipmentSlot);
		}
	}
	GemSlots.SetNum(MaxSKillGemCount);
	for (int32 i = 0; i < MaxSKillGemCount; ++i)
	{
		UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);
		GemSlots[i] = EquipmentSlot;
		GemSlots[i]->Init(this, i, EEquipmentType::SkillGem);
	}
}

TArray<TObjectPtr<UEquipmentInstance>> UItemManager::GetEquipments()
{
	TArray<TObjectPtr<UEquipmentInstance>> Ret;
	for (TObjectPtr<UEquipmentSlot>& EquipmentSlot : EquipmentSlots)
	{
		Ret.Add(Cast<UEquipmentInstance>(EquipmentSlot->GetItemInstance()));
	}
	return Ret;
}

void UItemManager::Clear()
{
	UnequipWeapon();
	for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
	{
		EquipmentSlots[i]->SetItemInstance(nullptr);
	}
	for (int32 i = 0; i < GemSlots.Num(); ++i)
	{
		GemSlots[i]->SetItemInstance(nullptr);
	}
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

EItemContainerType UItemManager::GetItemContainerType() const
{
	return EItemContainerType::Equipment;
}


UItemInstance* UItemManager::GetItem(int32 Index)
{
	if (Index < 0)
	{
		return nullptr;
	}

	if (Index < EquipmentSlots.Num())
	{
		if (EquipmentSlots[Index])
		{
			return EquipmentSlots[Index]->GetItemInstance();
		}
		return nullptr;
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		if (GemSlots[Index])
		{
			return GemSlots[Index]->GetItemInstance();
		}
		return nullptr;
	}
	return nullptr;
}

bool UItemManager::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance);
	if (!EquipmentInstance)
	{
		//TODO STAT And BroadCast
		return true;
	}
	if (Index < 0)
	{
		return false;
	}
	if (Index < EquipmentSlots.Num())
	{
		if (EquipmentSlots[Index] && EquipmentSlots[Index]->GetEquipmentType() == EquipmentInstance->GetEquipmentType())
		{
			//TODO STAT And BroadCast
			EquipmentSlots[Index]->SetItemInstance(EquipmentInstance);
		}
		if (UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset>(
			ItemInstance->GetItemDataAsset()))
		{
			if (EEquipmentType::Weapon == EquipmentItemDataAsset->GetEquipmentType())
			{
				EquipWeapon(EquipmentInstance);
			}
		}
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		if (!GemSlots.IsValidIndex(Index))
		{
			return false;
		}

		GemSlots[Index]->SetItemInstance(EquipmentInstance);
		if (AActor* Owner = GetOwner())
		{
			if (USkillManager* SkillComponent = Owner->FindComponentByClass<USkillManager>())
			{
				if (UGemItemDataAsset* GemItemDataAsset = Cast<
					UGemItemDataAsset>(EquipmentInstance->GetItemDataAsset()))
				{
					//TODO STAT And BroadCast
					SkillComponent->EquipSkillGem(Index + 1, GemItemDataAsset->GetSkillData());
				}
			}
		}
	}
	return true;
}


bool UItemManager::CanReceiveItem(UItemInstance* ItemInstance, int32 Index)
{
	if (Index < 0)
	{
		return false;
	}
	UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance);
	if (!EquipmentInstance)
	{
		return false;
	}
	if (Index < EquipmentSlots.Num())
	{
		if (EquipmentSlots[Index]->GetEquipmentType() != EquipmentInstance->GetEquipmentType())
		{
			return false;
		}
		return true;
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		if (EquipmentInstance->GetEquipmentType() != EEquipmentType::SkillGem)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool UItemManager::SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex)
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

void UItemManager::UnequipWeapon()
{
	if (IsValid(CurrentWeapon) == false)
		return;

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

UEquipmentSlot* UItemManager::GetSkillGemSlot(int32 Index)
{
	if (GemSlots.IsValidIndex(Index))
	{
		return GemSlots[Index];
	}
	return nullptr;
}

UEquipmentSlot* UItemManager::GetEquipmentSlot(EEquipmentType EquipmentType)
{
	int64 Index = static_cast<int64>(EquipmentType);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		return EquipmentSlots[Index];
	}
	return nullptr;
}


void UItemManager::RestoreEquipment(TArray<TObjectPtr<UEquipmentInstance>> EquipmentData)
{
	for (TObjectPtr<UEquipmentInstance>& EquipmentInstance : EquipmentData)
	{
		if (EquipmentInstance)
		{
			SetItemAt(EquipmentInstance, static_cast<int64>(EquipmentInstance->GetEquipmentType()));
		}
	}
}
