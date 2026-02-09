// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
	virtual void InitFromData() {}
	// 스킬 발동
	UFUNCTION(BlueprintCallable)
	virtual void Activate() {}
	// 쿨타임 체크
	UFUNCTION(BlueprintCallable)
	virtual bool IsActivate() { return true; }

protected:
	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|CooldownTime")
	float CooldownTime;
	// 마나 코스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|ManaCost")
	int32 ManaCost;
	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Damage")
	int32 Damage;
	// 스킬 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|DataTable")
	TObjectPtr<UDataTable> SkillTable;
};
