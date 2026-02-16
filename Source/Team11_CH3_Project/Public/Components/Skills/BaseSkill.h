// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Components/StatComponent.h"
#include "BaseSkill.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBaseSkill : public UObject
{
	GENERATED_BODY()

public:
	// 스킬 데이터 테이블에서 데이터 로딩
	UFUNCTION(BlueprintCallable)
	virtual void InitFromData(USkillDataAsset* Data) {}
	// 스킬 발동
	UFUNCTION(BlueprintCallable)
	virtual void Activate();
	// 쿨타임 체크
	UFUNCTION(BlueprintCallable)
	virtual bool IsActivate() { return true; }
	// 쿨타임 반환
	UFUNCTION(BlueprintCallable)
	virtual float GetCooldownTime() const { return CooldownTime; }

protected:
	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|CooldownTime")
	float CooldownTime;
	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Damage")
	float Damage;
	UPROPERTY()
	TObjectPtr<USkillDataAsset> SkillData = nullptr;  // 원본 데이터
};
