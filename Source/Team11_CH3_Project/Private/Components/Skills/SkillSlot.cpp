// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/BaseSkill.h"
#include "Engine/Engine.h"

void USkillSlot::EquipGem(TSubclassOf<class UBaseSkill> NewSkillClass)
{
	if (IsValid(NewSkillClass) == false)
		return;
	if (EquippedSkill)
	{
		ClearSlot();
		UE_LOG(LogTemp, Warning, TEXT("Clear EquipGem: %s"), *NewSkillClass->GetName());
	}

	EquippedSkill = NewObject<UBaseSkill>(this, NewSkillClass);
	UE_LOG(LogTemp, Warning, TEXT("EquipGem: %s"), *NewSkillClass->GetName());

}

void USkillSlot::ClearSlot()
{
	if (IsValid(EquippedSkill) == false)
		return;

	EquippedSkill = nullptr;

}
