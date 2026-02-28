// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "Components/Skills/SkillDataAsset.h"
#include "GemItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UGemItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:
	USkillDataAsset* GetSkillData()const{return SkillData.LoadSynchronous();}
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|SkillGem")
	TSoftObjectPtr<USkillDataAsset> SkillData;
};
