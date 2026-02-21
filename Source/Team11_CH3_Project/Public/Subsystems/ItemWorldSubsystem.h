// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/Items/PotionItemData.h"
#include "Components/Items/SkillGemItemData.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "Components/Items/Equipments/ArmorItemData.h"
#include "ItemWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FWeaponItemData* FindWeapon(FName RowName);
	FArmorItemData* FindArmor(FName RowName);
	FPotionItemData* FindPotion(FName RowName);
	FSkillGemItemData* FindSkillGem(FName RowName);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ArmorTable;
	UPROPERTY()
	TObjectPtr<UDataTable> PotionTable;
	UPROPERTY()
	TObjectPtr<UDataTable> SkillGemTable;
};
