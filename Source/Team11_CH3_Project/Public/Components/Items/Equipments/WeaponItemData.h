// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/EquipmentItemData.h"
#include "Types/StatTypes.h"
#include "WeaponItemData.generated.h"

class AWeaponActor;
class USkillDataAsset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FWeaponItemData : public FEquipmentItemData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item|Weapon")
	TSoftClassPtr<AWeaponActor> WeaponActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	TSoftObjectPtr<USkillDataAsset> DefaultSkill;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Weapon")
	float AttackRange = 200.0f;
};
