// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ActiveSkillSlot.h"

#include "Characters/PlayerCharacter.h"
#include "Characters/Monster/MonsterBase.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"

void UActiveSkillSlot::Init(USkillManager* SkillManager)
{
	SkillComponent = SkillManager;
}

void UActiveSkillSlot::OnStartSkill(AActor* InOwner, const FVector& InTargetLocation, USkillSlot* SkillSlot)
// 여기서 슬롯을 받게
{
	Owner = InOwner;
	TargetLocation = InTargetLocation;
	CurrentActiveSkillSlot = SkillSlot;
	CurrentActiveSkillSlot->GetEquippedSkill()->Enter();
	// Aiming 아닌경우 쿨타임 여기서
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() != ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
	}
	ElapsedTime = 0.0f;
	bIsEnd = false;
}

void UActiveSkillSlot::OnExecute()
{
	// Aiming인 경우에만 쿨타임시작
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
		bIsEnd = true;
	}
	CurrentActiveSkillSlot->GetEquippedSkill()->Execute();
}

void UActiveSkillSlot::OnTick(float DeltaSeconds)
{
	if (CurrentActiveSkillSlot == nullptr || CurrentActiveSkillSlot->GetEquippedSkill() == nullptr)
	{
		bIsEnd = true;
		return;
	}
	CurrentActiveSkillSlot->GetEquippedSkill()->Tick(DeltaSeconds, Owner.Get());
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
	CurrentActiveSkillSlot.Reset();
	CurrentActiveSkillSlot = nullptr;
	bIsEnd = false;
	Owner = nullptr;
	TargetLocation = FVector::ZeroVector;
}

void UActiveSkillSlot::Notify(FName NotifyName)
{
	if (!CurrentActiveSkillSlot.IsValid())
	{
		return;
	}
	//TODO Optimization
	AWeaponActor* WeaponActor = nullptr;
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner.Get()))
	{
		WeaponActor = PlayerCharacter->GetWeaponActor();
	}
	if (AMonsterBase* MonsterBase = Cast<AMonsterBase>(Owner.Get()))
	{
		WeaponActor = MonsterBase->GetWeaponActor();
	}
	CurrentActiveSkillSlot->GetEquippedSkill()->Notify(Cast<APawn>(Owner), WeaponActor, Owner->GetActorLocation(),
	                                                   (TargetLocation - Owner->GetActorLocation()).GetSafeNormal(),
	                                                   NotifyName);
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

void UActiveSkillSlot::SetIsEnd(bool isEnd)
{
	bIsEnd = isEnd;
}
