// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "Components/Items/Equipments/ArmorItemData.h"
#include "Components/StatComponent.h"
#include "T11_GameInstance.generated.h"


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stat")
	FStatData SavedStatData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Weapon")
	FWeaponItemData SavedWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Armor")
	TMap<EEquipmentType, FArmorItemData> SavedEquippedArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Skill")
	TArray<TSoftObjectPtr<USkillDataAsset>> SavedSkillSlots;
};
