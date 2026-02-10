// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillManager.h"
//#include "Components/Skills/BaseSkill.h"
#include "Components/Skills/BasicAttack.h"
#include "Components/Skills/SkillSlot.h"
#include "Components/Skills/Fireball.h"
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
	// ...
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
	if (IsValid(BasicAttack))
	{
		// 구현 예정
		BasicAttack->Activate();
	}
	else
	{
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, (TEXT("NoBasicAttack")));
		}
	}
}

void USkillManager::UseSkillSlot(int32 Index)
{
	if (SkillSlots.IsValidIndex(Index))
	{
		SkillSlots[Index]->GetEquippedSkill()->Activate();
	}
	else
	{
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("%d SkillSlotEmpty"), Index));
		}
	}
}

void USkillManager::EquipSkillGem(int32 SlotIndex, TSubclassOf<class UBaseSkill> NewSkillClass)
{
	if (NewSkillClass == nullptr)
		return;

	if (SkillSlots.IsValidIndex(SlotIndex))
	{
		SkillSlots[SlotIndex]->EquipGem(NewSkillClass);
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("EquipGem : %s"), *NewSkillClass->GetName()));
		}
	}
	else
	{
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("%d SkillSlotEmpty"), SlotIndex));
		}
	}
}

bool USkillManager::IsSkillOnCooldown(int32 SlotIndex) const
{
	return false;
}

float USkillManager::GetCooldownRemaining(int32 SlotIndex) const
{
	return 0.0f;
}

