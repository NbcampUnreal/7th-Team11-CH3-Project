#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/StatTypes.h"
#include "BuffManager.generated.h"

class UStatComponent;

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	Add,
	Multiply
};

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FBuffData
{
	GENERATED_BODY()

	UPROPERTY()
	FName BuffID;
	UPROPERTY()
	EStat TargetStat;
	UPROPERTY()
	EBuffType BuffType;
	UPROPERTY()
	float Amount;
	UPROPERTY()
	float Duration; // -1 = 무한
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UBuffManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffManager();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 버프 추가
	void AddBuff(const FBuffData& Buff);
	// 버프 제거
	void RemoveBuff(FName BuffID);

	// 버프 계산
	float CalculateBuffs(EStat Stat) const;

	// 버프 적용
	void SetBuffs(EStat Stat);

private:
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComp;

	UPROPERTY()
	TArray<FBuffData> ActiveBuffs;
};
