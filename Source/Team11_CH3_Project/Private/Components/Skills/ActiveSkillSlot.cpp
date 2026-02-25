// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ActiveSkillSlot.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"

void UActiveSkillSlot::Init(USkillManager* SkillManager)
{
	SkillComponent = SkillManager;
}

void UActiveSkillSlot::OnStartSkill(USkillSlot* Skill) // 여기서 슬롯을 받게
{
	CurrentActiveSkillSlot = Skill;
	CurrentActiveSkillSlot->GetEquippedSkill()->Enter();
	// Aiming 아닌경우 쿨타임 여기서
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() != ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
	}
	
	//CurrentActiveSkillSlot->Enter();
	ElapsedTime = 0.0f;
	bIsEnd = false;
}

void UActiveSkillSlot::OnExecute()
{
	// Aiming인 경우에만 쿨타임시작
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
	}

	CurrentActiveSkillSlot->GetEquippedSkill()->Execute();
	CurrentActiveSkillSlot->GetEquippedSkill()->OnExit();
	CurrentActiveSkillSlot = nullptr;
	bIsEnd = true;
}

void UActiveSkillSlot::OnTick(float DeltaSeconds, AActor* Owner)
{
	if (CurrentActiveSkillSlot == nullptr || CurrentActiveSkillSlot->GetEquippedSkill() == nullptr)
	{	
		bIsEnd = true;
		return;
	}
	CurrentActiveSkillSlot->GetEquippedSkill()->Tick(DeltaSeconds, Owner);
	ElapsedTime += DeltaSeconds;
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Duration)
	{
		bIsEnd = ElapsedTime > CurrentActiveSkillSlot->GetEquippedSkill()->GetDuration();
	}
}

void UActiveSkillSlot::OnExit()
{
	if (CurrentActiveSkillSlot == nullptr || IsValid(CurrentActiveSkillSlot->GetEquippedSkill()) == false)
		return;

	CurrentActiveSkillSlot->GetEquippedSkill()->OnExit();
	CurrentActiveSkillSlot = nullptr;
	bIsEnd = false;
}

float UActiveSkillSlot::GetElapsedTime() const
{
	return ElapsedTime;
}

bool UActiveSkillSlot::GetIsEnd()
{
	if (CurrentActiveSkillSlot == nullptr)
	{
		return true;
	}
	return bIsEnd;
}
