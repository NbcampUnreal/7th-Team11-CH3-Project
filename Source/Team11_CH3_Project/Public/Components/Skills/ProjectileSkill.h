// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/BaseSkill.h"
#include "ProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	void Activate() override;

protected:
	// 프로젝타일 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
