// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSlot.generated.h"

class USkillDataAsset;
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
	void EquipGem(USkillDataAsset* NewSkillData);
	// 슬롯 비우기
	UFUNCTION(BlueprintCallable)
	void ClearSlot();
	// 장착된 스킬 가져오기
	UFUNCTION(BlueprintCallable)
	USkillDataAsset* GetEquippedSkill() const { return EquippedSkill; }
	void StartCooldown();
	bool IsSkillOnCooldown()const;
	float GetCooldownRemaining() const;
	float GetScore(AActor* Actor, AActor* Target) const;

private:
	UPROPERTY()
	TObjectPtr<USkillDataAsset> EquippedSkill;
	
	FTimerHandle CooldownTimer;
	bool bIsOnCooldown;
};
