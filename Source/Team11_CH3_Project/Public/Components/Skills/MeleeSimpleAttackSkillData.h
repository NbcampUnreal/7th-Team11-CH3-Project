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
	                      const FVector& TargetLocation)  override;
	virtual float GetScore(const AActor* Actor, const AActor* Target) const override;
	virtual void Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation, FName Name)  override;
};
