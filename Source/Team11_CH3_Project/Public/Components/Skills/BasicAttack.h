// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/ProjectileSkill.h"
#include "BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBasicAttack : public UProjectileSkill
{
	GENERATED_BODY()

public:
	UBasicAttack();
	// 스킬 발동
	virtual void Activate() override;
};
