// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ItemWorldSubsystem.h"

void UItemWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 데이터 테이블 로드
	//WeaponTable   = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Weapon"));
	//ArmorTable    = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Armor"));
	//PotionTable   = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_Potion"));
	//SkillGemTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_SkillGem"));
}
//TODO Caching

