#include "Components/BuffComponent.h"
#include "Components/StatComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	checkf(StatComp, TEXT("No StatComponent Found"));
	CurrentBuffID = 0;

}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

int32 UBuffComponent::AddBuff(const EStat TargetStat, EBuffType BuffType, float Amount, float Duration)
{
	int32 NewID = ++CurrentBuffID;
	FBuffData Buff = { NewID, TargetStat, BuffType, Amount, Duration };
	ActiveBuffs.Add(Buff);
	SetBuffs(Buff.TargetStat);

	return NewID;
}

void UBuffComponent::RemoveBuff(int32 BuffID)
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

float UBuffComponent::CalculateBuffs(EStat Stat) const
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

void UBuffComponent::SetBuffs(EStat Stat)
{
	StatComp->SetCurrentStat(Stat, CalculateBuffs(Stat));
}