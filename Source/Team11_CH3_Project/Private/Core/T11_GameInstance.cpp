// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/T11_GameInstance.h"
#include "Components/ItemManager.h"
#include "Components/SkillManager.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Characters/InventoryComponent.h"
#include "Components/Items/ItemSlot.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Kismet/GameplayStatics.h"

void UT11_GameInstance::Init()
{
	InitData();
}

void UT11_GameInstance::InitData()
{
	CurrentStageIndex = 0;
	CurrentDifficulty = 0;
	TotalScore = 0;
}

void UT11_GameInstance::SavePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager, UInventoryComponent* InventoryComp)
{
	// 스탯 저장
	if (IsValid(StatComp) == false)
		return;

	// 아이템 데이터 저장
	if (IsValid(ItemManager) == false)
		return;
	// 장비 젬 슬롯
	SavedEquipmentsData.Empty();
	SavedEquipmentsData = ItemManager->GetEquipmentSaveData();
	SavedGemSlotsData.Empty();
	SavedGemSlotsData = ItemManager->GetGemSaveData();
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 장비 저장 - %d개 / 젬 %d개"),
		SavedEquipmentsData.Num(), SavedGemSlotsData.Num());

	// 현재 HP 저장
	SavedHPData = StatComp->GetCurrentHP();
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 현재 HP 저장 완료"));

	SavedInventoryData.Empty();
	if (IsValid(InventoryComp) == false)
		return;

	for (UItemSlot* Slot : InventoryComp->GetInventorySlots())
	{
		FSavedItemData ItemData;
		
		UItemInstance* Item = IsValid(Slot) ? Slot->GetItemInstance() : nullptr;
		if (IsValid(Item) && Item->IsValid())
		{
			ItemData.DataAsset = Item->GetItemDataAsset();
			ItemData.Count = Item->GetCount();
		}
		SavedInventoryData.Add(ItemData);
	}
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 인벤토리 저장 - %d슬롯"), SavedInventoryData.Num());


	SavedStatData = StatComp->BaseStat;
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 스탯 저장 완료"));

	// 저장 완료 다음 스테이지로
	CurrentStageIndex++;
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 저장 완료 - 다음 스테이지: %d"), CurrentStageIndex);
}

void UT11_GameInstance::RestorePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager, UInventoryComponent* InventoryComp)
{
	// 저장된 데이터 있는지 체크
	if (HasSavedData() == false)
		return;

	// 스탯 복구
	if (IsValid(StatComp) == false)
		return;
	StatComp->InitStat(SavedStatData);
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 스탯 복구 완료"));

	// 아이템 복구
	if (IsValid(ItemManager) == false)
		return;

	ItemManager->RestoreFromSaveData(SavedEquipmentsData);
	ItemManager->RestoreGemFromSaveData(SavedGemSlotsData);
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 장비/젬 복구 완료"));

	// 인벤토리 복구
	if (IsValid(InventoryComp) == false)
		return;

	for (int32 i = 0; i < SavedInventoryData.Num(); i++)
	{
		if (SavedInventoryData[i].DataAsset.IsNull())
			continue;

		UItemDataAsset* ItemData = SavedInventoryData[i].DataAsset.LoadSynchronous();
		if (IsValid(ItemData) == false)
			continue;

		UClass* ItemClass = ItemData->GetInstanceClass();
		UItemInstance* Instance = ItemClass
			? NewObject<UItemInstance>(this, ItemClass)
			: NewObject<UItemInstance>(this);

		if (UEquipmentInstance* EquipInstance = Cast<UEquipmentInstance>(Instance))
		{
			EquipInstance->Init(ItemData, 1);
		}
		else
		{
			Instance->Init(ItemData, SavedInventoryData[i].Count);
		}
		InventoryComp->SetItemAt(Instance, i);
	}

	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 인벤토리 복구 완료"));

	// HP 복구
	StatComp->SetCurrentHP(SavedHPData);

	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 전체 복구 완료 - 현재 스테이지: %d"), CurrentStageIndex);
}

void UT11_GameInstance::PlayInputSound(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(this, Sound, 0.5f);
	}
}