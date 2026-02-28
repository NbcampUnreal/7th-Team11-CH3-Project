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

void UItemManager::Equip(UEquipmentInstance* Data)
{
	//TODO STAT
	if (UEquipmentItemDataAsset* EquipmentItemDataAsset = Cast<UEquipmentItemDataAsset> (Data->GetItemDataAsset()))
	{
		if (EEquipmentType::Weapon ==  EquipmentItemDataAsset->GetEquipmentType())
		{
			EquipWeapon(Data);
		}
	}
	

}

void UItemManager::Unequip(EEquipmentType SlotType)
{
	//TODO STAT
	
}



void UItemManager::Clear()
{
	Unequip(EEquipmentType::Weapon);
	Unequip(EEquipmentType::Boots);
	Unequip(EEquipmentType::Chest);
	Unequip(EEquipmentType::Gloves);
	Unequip(EEquipmentType::Helmet);
	Unequip(EEquipmentType::Legs);
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
		Equipments.Add(EEquipmentType::Weapon,WeaponItemInstance);
		// PlayerCharacter의 WeaponActor 갱신
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
		{			
			if(USkillManager* SkillComponent = Player->FindComponentByClass<USkillManager>()){
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
		}
		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner()))
		{
			if(USkillManager* SkillComponent = Monster->FindComponentByClass<USkillManager>()){
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
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