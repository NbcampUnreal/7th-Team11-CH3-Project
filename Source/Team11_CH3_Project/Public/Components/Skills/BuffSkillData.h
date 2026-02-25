// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/SkillDataAsset.h"
#include "Components/BuffManager.h"
#include "BuffSkillData.generated.h"

/**
 * 
 */

UCLASS()
class TEAM11_CH3_PROJECT_API UBuffSkillData : public USkillDataAsset
{
	GENERATED_BODY()
	
protected:
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Skill|Buff")
	EStat TargetStat;

	UPROPERTY(EditDefaultsOnly, Category = "Skill|Buff")
	EBuffType BuffType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill|Buff")
	float BuffAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Skill|Buff")
	float BuffDuration; // -1 == 무한 

};
