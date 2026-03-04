// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillManager.h"
#include "Components/StatComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/Monster/MonsterBase.h"
#include "Components/Skills/ActiveSkillSlot.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Components/Skills/SkillSlot.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USkillManager::USkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillManager::Init()
{
	SkillSlots.Empty();
	for (int32 i = 0; i < 5; ++i)
	{
		USkillSlot* NewSlot = NewObject<USkillSlot>(this);
		NewSlot->Init(this, i);
		SkillSlots.Add(NewSlot);
	}
	ActiveSkillSlot = NewObject<UActiveSkillSlot>(this);
	ActiveSkillSlot->Init(this);
}

void USkillManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickActiveSkill(DeltaTime);
}


// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();
	Init();

}


TArray<int32> USkillManager::FindReadySlotIndexes() const
{
	TArray<int32> Ret;
	for (int32 i = 0; i < SkillSlots.Num(); i++)
	{
		if (SkillSlots.IsValidIndex(i) && IsValid(SkillSlots[i]->GetEquippedSkill()) && !SkillSlots[i]->
			IsSkillOnCooldown())
		{
			Ret.Add(i);
		}
	}
	return Ret;
}

int32 USkillManager::GetBestSkill(const AActor* Actor, const AActor* Target) const
{
	TArray<int32> ReadySkillIndexes = FindReadySlotIndexes();
	float MaxScore = -1;

	TArray<int32> BestSkillIdx;
	for (int32 i = 0; i < ReadySkillIndexes.Num(); i++)
	{
		float Score = SkillSlots[i]->GetScore(Actor, Target);
		if (MaxScore < Score)
		{
			MaxScore = Score;
			BestSkillIdx.Empty();
			BestSkillIdx.Add(i);
		}
		else if (MaxScore == Score)
		{
			BestSkillIdx.Add(i);
		}
	}
	if (MaxScore < 0.0f)
	{
		return -1;
	}
	return BestSkillIdx[FMath::RandRange(0, BestSkillIdx.Num() - 1)];
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
	OnSkillSlotChanged.Broadcast(SkillSlots[SlotIndex], true,false);
	UE_LOG(LogTemp, Warning, TEXT("EquipGem : %s"), *NewSkillData->GetName());
}

void USkillManager::UnEquipSkillGem(int32 SlotIndex)
{
	// 스킬 슬롯배열 체크
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d In Valid SKill Slot"), SlotIndex);
		return;
	}
	SkillSlots[SlotIndex]->ClearSlot();
	OnSkillSlotChanged.Broadcast(SkillSlots[SlotIndex], true,false);
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
	for (int32 i = 0; i < SkillSlots.Num();++i)
	{
		UnEquipSkillGem(i);
	}
	SkillSlots.Empty();
	if (IsSkillActive())
	{
		ExitActiveSkill();
	}
}

UActiveSkillSlot* USkillManager::GetActiveSkillSlot() const
{
	return ActiveSkillSlot.Get();
}

void USkillManager::ActiveSkill(AActor* Owner, AActor* Target, USkillSlot* SkillSlot)
{
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		SkeletalMeshComponent = Character->GetMesh();
		if (!IsValid(SkeletalMeshComponent))
		{
			SkeletalMeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
		}
	}
	if (!IsValid(SkeletalMeshComponent))
	{
		return;
	}


	UAnimMontage* SkillMontage = SkillSlot->GetEquippedSkill()->GetSkillMontage();
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	float CastSpeed = 1.0f;
	StatComp = GetOwner()->FindComponentByClass<UStatComponent>();

	if (StatComp.IsValid() == false)
		return;
	CastSpeed += StatComp->GetCurrentStat(EStat::CastSpeed);

	if (!IsValid(AnimInstance))
	{
		return;
	}
	float MontageCheck = AnimInstance->Montage_Play(SkillMontage, CastSpeed);
	if (MontageCheck <= 0.0f)
	{
		return;
	}
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &USkillManager::OnAttackMontageEnded);
	SkeletalMeshComponent->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);

	ActiveSkillSlot->OnStartSkill(Owner, Target, SkillSlot);
}

void USkillManager::TickActiveSkill(float DeltaSeconds)
{
	ActiveSkillSlot->OnTick(DeltaSeconds);
	if (ActiveSkillSlot->GetIsEnd())
	{
		ExitActiveSkill();
	}
}

void USkillManager::ExecuteActiveSkill()
{
	if (ActiveSkillSlot){
		ActiveSkillSlot->OnExecute();
	}
}

void USkillManager::ExitActiveSkill()
{
	ActiveSkillSlot->OnExit();
	if (AActor* Owner = GetOwner())
	{
		//TODO Delegate
		if (AMonsterBase* MonsterBase = Cast<AMonsterBase>(Owner))
		{
			MonsterBase->OnAttackEnded();
		}
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->OnAttackEnded();
		}
	}
}

bool USkillManager::IsSkillActive() const
{
	if (ActiveSkillSlot->GetSkillSlot())
	{
		return true;
	}
	return false;
}

void USkillManager::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (ActiveSkillSlot)
	{
		ActiveSkillSlot->SetIsEnd(true);
	}
}
