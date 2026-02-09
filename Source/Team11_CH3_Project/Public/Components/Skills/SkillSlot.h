// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API USkillSlot : public UObject
{
	GENERATED_BODY()
	
public:
	// 보석 장착
	UFUNCTION(BlueprintCallable)
	void EquipGem(TSubclassOf<class UBaseSkill> NewSkillClass);
	// 슬롯 비우기
	UFUNCTION(BlueprintCallable)
	void ClearSlot();
	// 장착된 스킬 가져오기
	UFUNCTION(BlueprintCallable)
	class UBaseSkill* GetEquippedSkill() const { return EquippedSkill; }

private:
	UPROPERTY()
	TObjectPtr<class UBaseSkill> EquippedSkill;
};
