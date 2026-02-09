// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/BaseSkill.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UFireball : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	// 스킬 데이터 테이블에서 데이터 로딩
	virtual void InitFromData() override;
	// 스킬 발동
	virtual void Activate() override;
	// 쿨타임 체크
	virtual bool IsActivate() { return true; }

};
