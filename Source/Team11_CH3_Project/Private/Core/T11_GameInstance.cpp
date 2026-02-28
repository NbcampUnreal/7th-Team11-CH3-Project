// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/T11_GameInstance.h"
#include "Components/ItemManager.h"
#include "Components/SkillManager.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"

void UT11_GameInstance::SavePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager)
{
	// 스탯 저장
	if (IsValid(StatComp) == false)
		return;
	SavedStatData = StatComp->CurrentStat;
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 스탯 저장 완료"));

	// 아이템 데이터 저장
	if (IsValid(ItemManager) == false)
		return;
	SavedEquipments = ItemManager->GetEquipments();
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 장비 저장 완료 - %d개"), SavedEquipments.Num());

	// 스킬 데이터 저장
	if (IsValid(SkillManager) == false)
		return;
	SavedSkillSlots.Empty();
	for (int32 i = 0; i < 2; i++)
	{
		USkillSlot* Slot = SkillManager->GetSkillSlot(i);
		if (IsValid(Slot) == false || IsValid(Slot->GetEquippedSkill()) == false)
			continue;

		SavedSkillSlots.Add(Slot->GetEquippedSkill());
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 스킬 저장 - Slot[%d]: %s"), i, *Slot->GetEquippedSkill()->GetName());
	}
	// 저장 완료 다음 스테이지로
	CurrentStageIndex++;
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 저장 완료 - 다음 스테이지: %d"), CurrentStageIndex);
}

void UT11_GameInstance::RestorePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager)
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
	ItemManager->RestoreEquipment(SavedEquipments);
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 장비 복구 완료 - %d개"), SavedEquipments.Num());
	// 스킬 복구
	if (IsValid(SkillManager) == false)
		return;
	for (int32 i = 0; i < SavedSkillSlots.Num(); i++)
	{
		USkillDataAsset* SkillData = SavedSkillSlots[i].LoadSynchronous();
		if (IsValid(SkillData))
		{
			SkillManager->EquipSkillGem(i, SkillData);
			UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 스킬 복구 - Slot[%d]: %s"), i, *SkillData->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] 복구 완료 - 현재 스테이지: %d"), CurrentStageIndex);
}
