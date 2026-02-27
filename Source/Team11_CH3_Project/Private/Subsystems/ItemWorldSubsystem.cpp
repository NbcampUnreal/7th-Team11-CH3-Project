// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ItemWorldSubsystem.h"

void UItemWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 데이터 테이블 로드
	WeaponTable   = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Weapon"));
	ArmorTable    = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Armor"));
	PotionTable   = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Potion"));
	SkillGemTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_SkillGem"));
}
//TODO Caching
FItemDataBase* UItemWorldSubsystem::FindItem(FDataTableRowHandle DataTableRowHandle)
{
	return DataTableRowHandle.GetRow<FItemDataBase>(TEXT("FindItem"));
}

FEquipmentItemData* UItemWorldSubsystem::FindEquipment(FDataTableRowHandle DataTableRowHandle)
{
	return DataTableRowHandle.GetRow<FEquipmentItemData>(TEXT("FindItem"));
}


FWeaponItemData* UItemWorldSubsystem::FindWeapon(FName RowName)
{
	
	return WeaponTable ? WeaponTable->FindRow<FWeaponItemData>(RowName, TEXT("FindWeapon")) : nullptr;
}

FArmorItemData* UItemWorldSubsystem::FindArmor(FName RowName)
{
	return ArmorTable ? ArmorTable->FindRow<FArmorItemData>(RowName, TEXT("FindArmor")) : nullptr;
}

FPotionItemData* UItemWorldSubsystem::FindPotion(FName RowName)
{
	return PotionTable ? PotionTable->FindRow<FPotionItemData>(RowName, TEXT("FindPotion")) : nullptr;
}

FSkillGemItemData* UItemWorldSubsystem::FindSkillGem(FName RowName)
{
	return SkillGemTable ? SkillGemTable->FindRow<FSkillGemItemData>(RowName, TEXT("FindSkillGem")) : nullptr;
}
