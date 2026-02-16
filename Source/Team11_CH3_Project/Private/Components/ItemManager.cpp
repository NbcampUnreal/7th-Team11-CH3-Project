// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManager.h"

// Sets default values for this component's properties
UItemManager::UItemManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UItemManager::UseItem(FName RowName)
{
}

void UItemManager::UnequipWeapon(FName RowName)
{
}

void UItemManager::UnequipArmor(FName RowName)
{
}


// Called when the game starts
void UItemManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UItemManager::UsePotion(FPotionItemData* Data)
{
}

void UItemManager::UseSkillGem(FSkillGemItemData* Data)
{
}

void UItemManager::EquipWeapon(FWeaponItemData* Data)
{
}

void UItemManager::EquipArmor(FArmorItemData* Data)
{
}


