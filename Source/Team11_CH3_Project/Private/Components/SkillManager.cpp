// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillManager.h"

#include "Components/Skills/SkillDataAsset.h"
#include "Components/Skills/SkillSlot.h"
#include "Engine/Engine.h"

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
	//if (AActor* Owner = GetOwner())
	//{
	//	StatComp = Owner->FindComponentByClass<UStatComponent>();
	//}
	// BasicAttack 생성


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


TArray<int32> USkillManager::FindReadySlotIdxes() const
{
	TArray<int32> Ret;
	for (int32 i = 0; i < SkillSlots.Num(); i++)
	{
		if (SkillSlots.IsValidIndex(i) && IsValid(SkillSlots[i]->GetEquippedSkill()) &&  !SkillSlots[i]->IsSkillOnCooldown())
		{
			Ret.Add(i);
		}
	}
	return Ret;
}

void USkillManager::StartSkillCooldown(int32 Index)
{
	// 예외 상황 체크
	if (SkillSlots.IsValidIndex(Index) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d In Valid SKill Slot"), Index);
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
	//// MP 체크 일단 주석처리
	//if (IsValid(StatComp))
	//{
	//	float CurrentMana = StatComp->GetCurrentStat(EStat::MP);
	//	float ManaCost = SkillSlots[Index]->GetEquippedSkill()->GetManaCost();

	//	if (CurrentMana < ManaCost)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Not Enough Mana! Current: % f, Cost %f"), CurrentMana, ManaCost);
	//		return;
	//	}

	//	StatComp->SetCurrentStat(EStat::MP, CurrentMana - ManaCost);
	//}
	// 스킬 발동
	SkillSlots[Index]->StartCooldown();
}

void USkillManager::EquipSkillGem(int32 SlotIndex, USkillDataAsset* NewSkillData)
{
	// 새로 장착할 Gem의 NewSkillClass nullptr 체크
	if (NewSkillData == nullptr)
		return;
	// 스킬 슬롯배열 체크
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d In Valid SKill Slot"), SlotIndex);
		return;
	}
	// 장착
	SkillSlots[SlotIndex]->EquipGem(NewSkillData);
	UE_LOG(LogTemp, Warning, TEXT("EquipGem : %s"), *NewSkillData->GetName());
}

bool USkillManager::IsSkillOnCooldown(int32 SlotIndex) const
{
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d In Valid SKill Slot"), SlotIndex);
		return false;
	}
	return SkillSlots[SlotIndex]->IsSkillOnCooldown();
}

float USkillManager::GetCooldownRemaining(int32 SlotIndex) const
{
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d In Valid SKill Slot"), SlotIndex);
		return FLT_MAX;
	}
	return SkillSlots[SlotIndex]->GetCooldownRemaining();
}
