// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Engine/Engine.h"

void USkillSlot::Init(USkillManager* SkillManager)
{
	SkillComponent = SkillManager;
}

void USkillSlot::EquipGem(const USkillDataAsset* NewSkillData)
{
	if (IsValid(NewSkillData) == false)
		return;
	if (EquippedSkill)
	{
		ClearSlot();
		UE_LOG(LogTemp, Warning, TEXT("Clear EquipGem: %s"), *NewSkillData->GetName());
	}
	USkillDataAsset* DuplicatedSKill =  DuplicateObject<USkillDataAsset>(NewSkillData,SkillComponent.Get());

	EquippedSkill = DuplicatedSKill;
	UE_LOG(LogTemp, Warning, TEXT("EquipGem: %s"), *DuplicatedSKill->GetName());
	bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,[this](){bIsOnCooldown = false;}, DuplicatedSKill->GetCooldownTime(),false);
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
	bIsOnCooldown = true;
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

float USkillSlot::GetScore(AActor* Actor, AActor* Target) const
{
	if (IsSkillOnCooldown())
		return -1;
	if (!GetEquippedSkill())
		return -1;
	return GetEquippedSkill()->GetScore(Actor,Target);
}

USkillManager* USkillSlot::GetSkillComponent() const
{
	return SkillComponent.Get();
}
