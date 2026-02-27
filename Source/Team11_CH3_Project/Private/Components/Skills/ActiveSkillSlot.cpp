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
	CurrentActiveSkillSlot->GetEquippedSkill()->Enter(InOwner,TargetLocation);
	// Aiming 아닌경우 쿨타임 여기서
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() != ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
	}
	ElapsedTime = 0.0f;
	SetIsEnd(false);
}

void UActiveSkillSlot::OnExecute()
{
	// Aiming인 경우에만 쿨타임시작
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		CurrentActiveSkillSlot->StartCooldown();
		CurrentActiveSkillSlot->GetEquippedSkill()->Execute();
	}
}

void UActiveSkillSlot::OnTick(float DeltaSeconds)
{
	if (CurrentActiveSkillSlot == nullptr || CurrentActiveSkillSlot->GetEquippedSkill() == nullptr)
	{
		SetIsEnd(true);
		return;
	}
	CurrentActiveSkillSlot->GetEquippedSkill()->Tick(DeltaSeconds, Owner.Get(), this);
	ElapsedTime += DeltaSeconds;
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Duration)
	{
		SetIsEnd(ElapsedTime > CurrentActiveSkillSlot->GetEquippedSkill()->GetDuration());
	}
}

void UActiveSkillSlot::OnExit()
{
	// TODO 나중엔 여기서 애님인스턴스 정리하지말고 SkillDataAsset쪽에서 정리하고 
	// 여기서는 SkillDataAsset쪽의 OnExit()호출만
	if (CurrentActiveSkillSlot == nullptr || IsValid(CurrentActiveSkillSlot->GetEquippedSkill()) == false)
		return;

	CurrentActiveSkillSlot->GetEquippedSkill()->OnExit();
	CurrentActiveSkillSlot.Reset();
	CurrentActiveSkillSlot = nullptr;
	SetIsEnd(false);
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
	                                                   TargetLocation, NotifyName);
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
