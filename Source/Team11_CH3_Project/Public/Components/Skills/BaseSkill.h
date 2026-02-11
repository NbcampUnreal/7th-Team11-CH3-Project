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
	// 스킬 데이터 테이블에서 데이터 로딩(도전 기능때 구현)
	UFUNCTION(BlueprintCallable)
	virtual void InitFromData() {}
	// 스킬 발동
	UFUNCTION(BlueprintCallable)
	virtual void Activate();
	// 쿨타임 체크
	UFUNCTION(BlueprintCallable)
	virtual bool IsActivate() { return true; }
	// 쿨타임 반환
	UFUNCTION(BlueprintCallable)
	virtual float GetCooldownTime() const { return CooldownTime; }
	// 마나 코스트 반환
	UFUNCTION(BlueprintCallable)
	virtual int32 GetManaCost() const { return ManaCost; }

protected:
	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|CooldownTime")
	float CooldownTime;
	// 마나 코스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|ManaCost")
	int32 ManaCost;
	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Damage")
	int32 Damage;
	// 스킬 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|DataTable")
	TObjectPtr<UDataTable> SkillTable;
	// 프로젝타일 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
