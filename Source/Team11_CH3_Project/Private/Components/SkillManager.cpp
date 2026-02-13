// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillManager.h"
#include "Components/Skills/BasicAttack.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/Fireball.h"
#include "Engine/Engine.h"
#include "Components/StatComponent.h"

// Sets default values for this component's properties
USkillManager::USkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();
	// StatComponent 연동
	if (AActor* Owner = GetOwner())
	{
		StatComp = Owner->FindComponentByClass<UStatComponent>();
	}
	// BasicAttack 생성
	if (IsValid(BasicAttackClass))
	{
		BasicAttack = NewObject<UBasicAttack>(this, BasicAttackClass);
	}

	SkillSlots.Empty();
	for (const auto& SkillClass : DefaultSkillSlotClasses)
	{
		if (IsValid(SkillClass))
		{
			USkillSlot* NewSlot = NewObject<USkillSlot>(this);
			NewSlot->EquipGem(SkillClass);
			SkillSlots.Add(NewSlot);
		}
	}
}


// Called every frame
//void USkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void USkillManager::UseBasicAttack()
{
	// BasicAttack 체크
	if (IsValid(BasicAttack) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoBasicAttack"));
		return;
	}
	// 기본공격
	BasicAttack->Activate();

}

void USkillManager::UseSkillSlot(int32 Index)
{
	// 예외 상황 체크
	if (SkillSlots.IsValidIndex(Index) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d SkillSlotEmpty"), Index);
		return;
	}
	if (SkillSlots[Index] == nullptr || SkillSlots[Index]->GetEquippedSkill() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d Skill Is Empty"), Index);
		return;
	}
	// 쿨타임 체크
	if (IsSkillOnCooldown(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill On Cooldown: %.1fs"), GetCooldownRemaining(Index));
		return;
	}
	// MP 체크
	if (IsValid(StatComp))
	{
		float CurrentMana = StatComp->GetCurrentStat(EStat::MP);
		float ManaCost = SkillSlots[Index]->GetEquippedSkill()->GetManaCost();

		if (CurrentMana < ManaCost)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Enough Mana! Current: %f, Cost: %f"), CurrentMana, ManaCost);
			return;
		}

		StatComp->SetCurrentStat(EStat::MP, CurrentMana - ManaCost);
	}
	// 스킬 발동
	SkillSlots[Index]->GetEquippedSkill()->Activate();
	// 쿨타임 시작
	float CooldownTime = SkillSlots[Index]->GetEquippedSkill()->GetCooldownTime();
	if (CooldownTime > 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &USkillManager::OnCooldownFinished, Index),
			CooldownTime,
			false
		);
		// 각 인덱스에 쿨타임 저장
		CooldownTimers.Emplace(Index, TimerHandle);
	}
}

void USkillManager::EquipSkillGem(int32 SlotIndex, TSubclassOf<class UBaseSkill> NewSkillClass)
{
	// 새로 장착할 Gem의 NewSkillClass nullptr 체크
	if (NewSkillClass == nullptr)
		return;
	// 스킬 슬롯배열 체크
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d SkillSlotEmpty"), SlotIndex);
		return;
	}
	// 장착
	SkillSlots[SlotIndex]->EquipGem(NewSkillClass);
	UE_LOG(LogTemp, Warning, TEXT("EquipGem : %s"), *NewSkillClass->GetName());
}

bool USkillManager::IsSkillOnCooldown(int32 SlotIndex) const
{
	// CooldownTimers의 인덱스가 nullptr이 아니고 쿨타임이 남아있다면 true 반환
	if (const FTimerHandle* Handle = CooldownTimers.Find(SlotIndex))
	{
		return GetWorld()->GetTimerManager().IsTimerActive(*Handle);
	}
	return false;
}

float USkillManager::GetCooldownRemaining(int32 SlotIndex) const
{
	// CooldownTimers의 인덱스가 nullptr이 아니고 쿨타임이 남아있다면 남은 시간 반환
	if (const FTimerHandle* Handle = CooldownTimers.Find(SlotIndex))
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(*Handle);
	}
	return 0.0;
}

void USkillManager::OnCooldownFinished(int32 SlotIndex)
{
	// 쿨타임이 끝나면 인덱스 삭제
	CooldownTimers.Remove(SlotIndex);
}

