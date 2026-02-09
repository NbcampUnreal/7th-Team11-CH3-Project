// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/BaseSkill.h"
#include "Engine/Engine.h"

void USkillSlot::EquipGem(TSubclassOf<class UBaseSkill> NewSkillClass)
{
	if (IsValid(NewSkillClass) == false)
		return;

	if (EquippedSkill != nullptr)
	{
		ClearSlot();
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Clear EquipGem : %s"), *NewSkillClass->GetName()));
		}
	}

	EquippedSkill = NewObject<UBaseSkill>(this, NewSkillClass);
	if (IsValid(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("EquipGem : %s"), *NewSkillClass->GetName()));
	}
}

void USkillSlot::ClearSlot()
{
	if (IsValid(EquippedSkill))
	{
		EquippedSkill = nullptr;
	}
}
