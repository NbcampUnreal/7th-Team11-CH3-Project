#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

UENUM(BlueprintType)
enum class EStat : uint8
{
	HP,
	MaxHP,
	DEF,
	MoveSpeed,
	AttackDamage,
	CastSpeed,
	ProjectileSpeed,
	CriticalChance,
	CriticalDamage,
};

USTRUCT()
struct TEAM11_CH3_PROJECT_API FStatData
{
	GENERATED_BODY()

	UPROPERTY()
	float HP;
	UPROPERTY()
	float MaxHP;
	UPROPERTY()
	float DEF;
	UPROPERTY()
	float MoveSpeed;
	UPROPERTY()
	float AttackDamage;
	UPROPERTY()
	float CastSpeed;
	UPROPERTY()
	float ProjectileSpeed;
	UPROPERTY()
	float CriticalChance;
	UPROPERTY()
	float CriticalDamage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	virtual void BeginPlay() override;

	// 기본 스탯 설정
	void SetBaseStat(const FStatData& StatData);
	// 기본 특정 스탯 설정
	void SetBaseStat(EStat TargetStat, float Amount);
	// 현재 특정 스탯 설정
	void SetCurrentStat(EStat TargetStat, float Amount);

	// 기본 스탯 가져오기
	float GetBaseStat(EStat TargetStat) const;
	// 현재 스탯 가져오기
	float GetCurrentStat(EStat TargetStat) const;

	// 받는 데미지 계산
	bool TakeDamage(float DamageAmount);
		
public:
	UPROPERTY()
	FStatData CurrentStat;

private:
	UPROPERTY()
	FStatData BaseStat;
};
