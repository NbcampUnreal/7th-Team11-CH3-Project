// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName SkillID;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float CooldownTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class UBaseSkill> SkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SkillMontage = nullptr;
};
