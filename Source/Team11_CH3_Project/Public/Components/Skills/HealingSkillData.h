// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/SkillDataAsset.h"
#include "HealingSkillData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UHealingSkillData : public USkillDataAsset
{
	GENERATED_BODY()
protected:
	void DoHeal();
	virtual void Notify(APawn* Instigator, AWeaponActor* WeaponActor,
		const FVector& Origin, const FVector& TargetLocation, FName Name) override;

	// 즉시 회복할 HP량
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Heal")
	float HealAmount = 50.f;
};
