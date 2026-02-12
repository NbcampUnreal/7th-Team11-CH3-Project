// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/EquipmentItemData.h"
#include "WeaponItemData.generated.h"

class AWeaponActor;
/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FWeaponItemData : public FEquipmentItemData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item|Weapon")
	TSoftClassPtr<AWeaponActor> WeaponActorClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item|Weapon")
	float Damage;	
};
