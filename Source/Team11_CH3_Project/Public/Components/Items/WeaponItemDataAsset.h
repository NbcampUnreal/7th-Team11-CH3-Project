// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemDataAsset.h"
#include "ItemDataAsset.h"
#include "WeaponActor.h"
#include "WeaponItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UWeaponItemDataAsset : public UEquipmentItemDataAsset
{
	GENERATED_BODY()

public:
	TSoftClassPtr<AWeaponActor> GetWeaponActorClass() { return WeaponActorClass; }
	EWeaponType GetWeaponType() const { return WeaponType; }
	USkillDataAsset* GetDefaultSkill() const { return DefaultSkill.LoadSynchronous(); }
	float GetAttackRange() const { return AttackRange; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	TSoftClassPtr<AWeaponActor> WeaponActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	TSoftObjectPtr<USkillDataAsset> DefaultSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	float AttackRange = 200.0f;
};
