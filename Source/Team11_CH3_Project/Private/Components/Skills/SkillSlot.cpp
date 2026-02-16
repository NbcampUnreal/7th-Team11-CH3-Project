// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/BaseSkill.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Engine/Engine.h"

void USkillSlot::EquipGem(USkillDataAsset* NewSkillData)
{
	if (IsValid(NewSkillData) == false || IsValid(NewSkillData->SkillClass) == false)
		return;
	if (EquippedSkill)
	{
		ClearSlot();
		UE_LOG(LogTemp, Warning, TEXT("Clear EquipGem: %s"), *NewSkillData->SkillName.ToString());
	}

	EquippedSkill = NewObject<UBaseSkill>(this, NewSkillData->SkillClass);
	EquippedSkill->InitFromData(NewSkillData);
	UE_LOG(LogTemp, Warning, TEXT("EquipGem: %s"), *NewSkillData->SkillName.ToString());

}

void USkillSlot::ClearSlot()
{
	if (IsValid(EquippedSkill) == false)
		return;

	EquippedSkill = nullptr;

}
