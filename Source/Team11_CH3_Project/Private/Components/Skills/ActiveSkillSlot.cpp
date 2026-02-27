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

	ACharacter* Character = Cast<ACharacter>(Owner);
	if (IsValid(Character) == false)
		return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	UAnimMontage* SkillMontage = CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillMontage();
	if (!IsValid(AnimInstance))
		return;

	// Aiming인 경우에만 쿨타임시작
	if (CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		AnimInstance->Montage_JumpToSection("EndCasting", SkillMontage);
		CurrentActiveSkillSlot->StartCooldown();
	}
	//CurrentActiveSkillSlot->GetEquippedSkill()->Execute();
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
	if (CurrentActiveSkillSlot == nullptr || IsValid(CurrentActiveSkillSlot->GetEquippedSkill()) == false)
		return;
	ACharacter* Character = Cast<ACharacter>(Owner.Get());
	if (IsValid(Character) == false)
		return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	UAnimMontage* SkillMontage = CurrentActiveSkillSlot->GetEquippedSkill()->GetSkillMontage();
	if (IsValid(AnimInstance) == false || IsValid(SkillMontage) == false)
		return;
	AnimInstance->Montage_Stop(0.2f, SkillMontage);

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
