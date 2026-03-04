// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/StatComponent.h"
#include "Types/ItemTypes.h"
#include "Types/SaveTypes.h" 
#include "T11_GameInstance.generated.h"


class UEquipmentInstance;
class UEquipmentComponent;
class USkillComponent;
class USkillDataAsset;
class UItemInstance;
class UInventoryComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API UT11_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	void InitData();
	// 플레이어 데이터 저장 및 복구
	UFUNCTION(BlueprintCallable)
	void SavePlayerData(UStatComponent* StatComp, UEquipmentComponent* ItemManager, USkillComponent* SkillManager, UInventoryComponent* InventoryComp);
	UFUNCTION(BlueprintCallable)
	void RestorePlayerData(UStatComponent* StatComp, UEquipmentComponent* ItemManager, USkillComponent* SkillManager, UInventoryComponent* InventoryComp);
	UFUNCTION(BlueprintCallable)
	bool HasSavedData() const { return CurrentStageIndex > 0; }

	UFUNCTION(BlueprintCallable)
	void PlayInputSound(class USoundBase* Sound);

	// 저장 된 데이터 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 MaxStageCount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 CurrentStageIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 CurrentDifficulty; // 0 == Normal, 1 == Hard Enum으로 설정하는 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stage")
	int32 MidBossStage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stat")
	FStatData SavedStatData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stat")
	float SavedHPData;
	// 장착 장비
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Eqiupment")
	TArray<FSavedEquipmentData> SavedEquipmentsData;
	// 스킬 젬 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Skill")
	TArray<FSavedEquipmentData> SavedGemSlotsData;
	// 인벤토리 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TArray<FSavedItemData> SavedInventoryData;
	// 점수 데이터 총 점수 스테이지 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Score")
	int32 TotalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Score")
	TMap<int32, int32> StageScore;
};
