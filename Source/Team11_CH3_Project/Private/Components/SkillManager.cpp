// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillManager.h"

#include "Components/Skills/ActiveSkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Components/Skills/SkillSlot.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
USkillManager::USkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickActiveSkill(DeltaTime,GetOwner());
}


// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();


	SkillSlots.Empty();
	// for (USkillDataAsset* SkillData : DefaultSkillSlotData)
	for (int32 i = 0;i<3;++i)
	{
		USkillSlot* NewSlot = NewObject<USkillSlot>(this);
		NewSlot->Init(this);
		SkillSlots.Add(NewSlot);
		// if (IsValid(SkillData))
		// {
		// 	USkillSlot* NewSlot = NewObject<USkillSlot>(this);
		// 	NewSlot->EquipGem(SkillData);
		// 	SkillSlots.Add(NewSlot);
		// }
	}
	ActiveSkillSlot = NewObject<UActiveSkillSlot>(this);
	ActiveSkillSlot->Init(this);
}


// Called every frame
//void USkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}


TArray<int32> USkillManager::FindReadySlotIndexes() const
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

int32 USkillManager::GetBestSkill(AActor* Actor, AActor* Target) const
{
	TArray<int32> ReadySkillIndexes = FindReadySlotIndexes();
	float MaxScore = -1;
	int32 Ret=-1;
	for (int32 i = 0; i < ReadySkillIndexes.Num(); i++)
	{
		float Score = SkillSlots[i]->GetScore(Actor,Target);
		if (MaxScore < Score)
		{
			MaxScore = Score;
			Ret = i;
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

	// 스킬 발동
	SkillSlots[Index]->StartCooldown();
}

void USkillManager::EquipSkillGem(int32 SlotIndex, USkillDataAsset* NewSkillData)
{
	// 새로 장착할 Gem의 NewSkillData 체크
	if (IsValid(NewSkillData) == false)
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

void USkillManager::AddSKillGems(TArray<TSoftObjectPtr<USkillDataAsset>> Skills)
{
	for (TSoftObjectPtr<USkillDataAsset> SkillData : Skills)
	{
		if (IsValid(SkillData.LoadSynchronous()))
		{
			USkillSlot* NewSlot = NewObject<USkillSlot>(this);
			NewSlot->EquipGem(SkillData.Get());
			SkillSlots.Add(NewSlot);
		}
	}
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

void USkillManager::Clear()
{
	SkillSlots.Empty();
}

UActiveSkillSlot* USkillManager::GetActiveSkillSlot() const
{
	return ActiveSkillSlot.Get();
}

void USkillManager::ActiveSkill(USkillDataAsset* Skill) const
{
	ActiveSkillSlot->OnStartSkill(Skill);
}

void USkillManager::TickActiveSkill(float DeltaSeconds, AActor* Owner) const
{
	ActiveSkillSlot->OnTick(DeltaSeconds, Owner);
	if (ActiveSkillSlot->GetIsEnd())
	{
		ExitActiveSkill();
	}
}

void USkillManager::ExecuteActiveSkill() const
{
	ActiveSkillSlot->OnExecute();
}

void USkillManager::ExitActiveSkill() const
{
	ActiveSkillSlot->OnExit();
}
