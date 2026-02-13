#include "Components/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UStatComponent::InitStat(const FStatData& StatData)
{
	BaseStat = StatData;
	CurrentStat = BaseStat;
	CurrentHP = BaseStat.MaxHP;
}

void UStatComponent::SetBaseStat(EStat TargetStat, float Amount)
{
	switch (TargetStat)
	{
	case EStat::MaxHP:
		BaseStat.MaxHP = FMath::Max(Amount, 0.0f);
		break;
	case EStat::DEF:
		BaseStat.DEF = Amount;
		break;
	case EStat::MoveSpeed:
		BaseStat.MoveSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::AttackDamage:
		BaseStat.AttackDamage = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CastSpeed:
		BaseStat.CastSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::ProjectileSpeed:
		BaseStat.ProjectileSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CriticalChance:
		BaseStat.CriticalChance = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CriticalDamage:
		BaseStat.CriticalDamage = FMath::Max(Amount, 1.0f);
		break;
	default:
		break;
	}
}

void UStatComponent::SetCurrentStat(EStat TargetStat, float Amount)
{
	switch (TargetStat)
	{
	case EStat::MaxHP:
		CurrentStat.MaxHP = FMath::Max(Amount, 0.0f);
		break;
	case EStat::DEF:
		CurrentStat.DEF = Amount;
		break;
	case EStat::MoveSpeed:
		CurrentStat.MoveSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::AttackDamage:
		CurrentStat.AttackDamage = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CastSpeed:
		CurrentStat.CastSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::ProjectileSpeed:
		CurrentStat.ProjectileSpeed = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CriticalChance:
		CurrentStat.CriticalChance = FMath::Max(Amount, 0.0f);
		break;
	case EStat::CriticalDamage:
		CurrentStat.CriticalDamage = FMath::Max(Amount, 1.0f);
		break;
	default:
		break;
	}
}

float UStatComponent::GetBaseStat(EStat TargetStat) const
{
	switch (TargetStat)
	{
	case EStat::MaxHP:
		return BaseStat.MaxHP;
	case EStat::DEF:
		return BaseStat.DEF;
	case EStat::MoveSpeed:
		return BaseStat.MoveSpeed;
	case EStat::AttackDamage:
		return BaseStat.AttackDamage;
	case EStat::CastSpeed:
		return BaseStat.CastSpeed;
	case EStat::ProjectileSpeed:
		return BaseStat.ProjectileSpeed;
	case EStat::CriticalChance:
		return BaseStat.CriticalChance;
	case EStat::CriticalDamage:
		return BaseStat.CriticalDamage;
	default:
		return 0.0f;
	}
}

float UStatComponent::GetCurrentStat(EStat TargetStat) const
{
	switch (TargetStat)
	{
	case EStat::MaxHP:
		return CurrentStat.MaxHP;
	case EStat::DEF:
		return CurrentStat.DEF;
	case EStat::MoveSpeed:
		return CurrentStat.MoveSpeed;
	case EStat::AttackDamage:
		return CurrentStat.AttackDamage;
	case EStat::CastSpeed:
		return CurrentStat.CastSpeed;
	case EStat::ProjectileSpeed:
		return CurrentStat.ProjectileSpeed;
	case EStat::CriticalChance:
		return CurrentStat.CriticalChance;
	case EStat::CriticalDamage:
		return CurrentStat.CriticalDamage;
	default:
		return 0.0f;
	}
}

void UStatComponent::SetCurrentHP(float Amount)
{
	CurrentHP = FMath::Max(Amount, 0.0f);
}

void UStatComponent::AddCurrentHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, GetCurrentStat(EStat::MaxHP));
}

float UStatComponent::GetCurrentHP() const
{
	return CurrentHP;
}

bool UStatComponent::TakeDamage(float DamageAmount)
{
	float CalculatedDamage = DamageAmount*100/FMath::Max(100 + GetCurrentStat(EStat::DEF), 0.01f); // 데미지 * 100/(100+방어력)
	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), CalculatedDamage);
	AddCurrentHP(-CalculatedDamage);

	if (GetCurrentHP() <= 0)
	{
		// 사망
		return true;
	}
	return false;
}