// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/ProjectileSkill.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UFireball : public UProjectileSkill
{
	GENERATED_BODY()
	
public:
	UFireball();
	// 스킬 발동
	virtual void Activate() override;
	// 쿨타임 체크
	virtual bool IsActivate() { return true; }
};
