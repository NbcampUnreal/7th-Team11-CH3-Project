#include "Components/BuffManager.h"
#include "Components/StatComponent.h"

UBuffManager::UBuffManager()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffManager::BeginPlay()
{
	Super::BeginPlay();

	StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	checkf(StatComp, TEXT("No StatComponent Found"));
	CurrentBuffID = 0;

}

void UBuffManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i--)
	{
		if (ActiveBuffs[i].Duration == -1.0f)
			continue;

		ActiveBuffs[i].Duration -= DeltaTime;

		if (ActiveBuffs[i].Duration <= 0.0f)
		{
			EStat DeletedStat = ActiveBuffs[i].TargetStat;
			ActiveBuffs.RemoveAt(i);
			SetBuffs(DeletedStat);
		}
	}
}

int32 UBuffManager::AddBuff(const EStat TargetStat, EBuffType BuffType, float Amount, float Duration)
{
	int32 NewID = ++CurrentBuffID;
	FBuffData Buff = { NewID, TargetStat, BuffType, Amount, Duration };
	ActiveBuffs.Add(Buff);
	SetBuffs(Buff.TargetStat);

	return NewID;
}

void UBuffManager::RemoveBuff(int32 BuffID)
{
	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i--)
	{
		if (ActiveBuffs[i].BuffID == BuffID)
		{
			EStat DeletedStat = ActiveBuffs[i].TargetStat;
			ActiveBuffs.RemoveAt(i);
			SetBuffs(DeletedStat);
		}
	}
}

float UBuffManager::CalculateBuffs(EStat Stat) const
{
	float Adds = 0.0f;
	float Muls = 1.0f;

	for (const FBuffData& Buff : ActiveBuffs)
	{
		if (Buff.TargetStat != Stat) continue;

		if (Buff.BuffType == EBuffType::Add)
		{
			Adds += Buff.Amount;
		}
		else if (Buff.BuffType == EBuffType::Multiply)
		{
			Muls *= Buff.Amount;
		}
	}

	return (StatComp->GetBaseStat(Stat) + Adds) * Muls;
}

void UBuffManager::SetBuffs(EStat Stat)
{
	StatComp->SetCurrentStat(Stat, CalculateBuffs(Stat));
}