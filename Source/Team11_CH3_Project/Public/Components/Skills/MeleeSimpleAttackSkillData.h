// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataAsset.h"
#include "MeleeSimpleAttackSkillData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UMeleeSimpleAttackSkillData : public USkillDataAsset
{
	GENERATED_BODY()

public:
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	                      const FVector& Direction) const override;
	virtual float GetScore(AActor* Actor, AActor* Target) const override;
};
