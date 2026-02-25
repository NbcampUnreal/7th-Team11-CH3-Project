// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ActiveSkillSlot.h"

#include "Components/Skills/SkillDataAsset.h"

void UActiveSkillSlot::Init(USkillManager* SkillManager)
{
	SkillComponent = SkillManager;
}

void UActiveSkillSlot::OnStartSkill(USkillDataAsset* Skill)
{
	CurrentSKill = Skill;
	CurrentSKill->Enter();
	ElapsedTime = 0.0f;
}

void UActiveSkillSlot::OnExecute()
{
	CurrentSKill->Execute();
	bIsEnd = true;
}

void UActiveSkillSlot::OnTick(float DeltaSeconds, AActor* Owner)
{
	CurrentSKill->Tick(DeltaSeconds, Owner);
	ElapsedTime += DeltaSeconds;
	if (CurrentSKill->GetSkillType() == ESkillType::Duration)
	{
		bIsEnd = ElapsedTime > CurrentSKill->GetDuration();
	}
}

void UActiveSkillSlot::OnExit()
{
	CurrentSKill = nullptr;
	CurrentSKill->OnExit();
}

float UActiveSkillSlot::GetElapsedTime() const
{
	return ElapsedTime;
}

bool UActiveSkillSlot::GetIsEnd()
{
	if (CurrentSKill == nullptr)
	{
		return true;
	}
	return bIsEnd;
}
