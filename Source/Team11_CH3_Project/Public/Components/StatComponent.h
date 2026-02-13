#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/StatTypes.h"
#include "StatComponent.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	virtual void BeginPlay() override;

	// 스탯 초기화
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void InitStat(const FStatData& StatData);

	// 기본 특정 스탯 설정
	void SetBaseStat(EStat TargetStat, float Amount);
	// 현재 특정 스탯 설정
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SetCurrentStat(EStat TargetStat, float Amount);

	// 기본 스탯 가져오기
	UFUNCTION(BlueprintCallable, Category = "Stat")
	float GetBaseStat(EStat TargetStat) const;
	// 현재 스탯 가져오기
	UFUNCTION(BlueprintCallable, Category = "Stat")
	float GetCurrentStat(EStat TargetStat) const;

	// 현재 체력 초기화
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SetCurrentHP(float Amount);
	// 현재 체력 증감
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void AddCurrentHP(float Amount);
	// 현재 체력 가져오기
	UFUNCTION(BlueprintCallable, Category = "Stat")
	float GetCurrentHP() const;

	// 받는 데미지 계산
	UFUNCTION(BlueprintCallable, Category = "Stat")
	bool TakeDamage(float DamageAmount);
		
public:
	UPROPERTY()
	FStatData CurrentStat;

private:
	UPROPERTY()
	FStatData BaseStat;

	UPROPERTY()
	float CurrentHP;
};
