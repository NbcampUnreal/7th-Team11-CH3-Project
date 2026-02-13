// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/ItemDataBase.h"
#include "SkillGemItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FSkillGemItemData : public FItemDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|SkillGem")
	TSubclassOf<class UBaseSkill>SkillClass;
};
