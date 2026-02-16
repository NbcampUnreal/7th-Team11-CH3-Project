// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Engine/Engine.h"

void USkillSlot::EquipGem(USkillDataAsset* NewSkillData)
{
	if (IsValid(NewSkillData) == false)
		return;
	if (EquippedSkill)
	{
		ClearSlot();
		UE_LOG(LogTemp, Warning, TEXT("Clear EquipGem: %s"), *NewSkillData->GetName());
	}

	EquippedSkill = NewSkillData;
	UE_LOG(LogTemp, Warning, TEXT("EquipGem: %s"), *NewSkillData->GetName());
	bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,[this](){bIsOnCooldown = false;}, NewSkillData->GetCooldownTime(),false);
}

void USkillSlot::ClearSlot()
{
	if (IsValid(EquippedSkill) == false)
		return;

	EquippedSkill = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);

}

void USkillSlot::StartCooldown()
{
	if (!IsValid(EquippedSkill))
	{
		return;	
	}
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,[this](){bIsOnCooldown = false;}, EquippedSkill->GetCooldownTime(),false);
	
}

bool USkillSlot::IsSkillOnCooldown() const
{
	return bIsOnCooldown;
}

float USkillSlot::GetCooldownRemaining() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer);
}
