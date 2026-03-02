// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/StatComponent.h"
#include "Types/ItemTypes.h"
#include "T11_GameInstance.generated.h"


class UEquipmentInstance;
/**
 * 
 */
class UItemManager;
class USkillManager;
class USkillDataAsset;

UCLASS()
class TEAM11_CH3_PROJECT_API UT11_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 플레이어 데이터 저장 및 복구
	UFUNCTION(BlueprintCallable)
	void SavePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager);
	UFUNCTION(BlueprintCallable)
	void RestorePlayerData(UStatComponent* StatComp, UItemManager* ItemManager, USkillManager* SkillManager);
	UFUNCTION(BlueprintCallable)
	bool HasSavedData() const { return CurrentStageIndex > 0; }

	// 저장 된 데이터 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 CurrentStageIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 CurrentDifficulty = 0; // 0 == Normal, 1 == Hard Enum으로 설정하는 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stat")
	FStatData SavedStatData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stat")
	float SavedHPData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Armor")
	TArray<TObjectPtr<UEquipmentInstance>> SavedEquipments;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Skill")
	TArray<TSoftObjectPtr<USkillDataAsset>> SavedSkillSlots;
	// 점수 데이터 총 점수 스테이지 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Score")
	int32 TotalScore = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Score")
	TMap<int32, int32> StageScore;
};
