// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/SkillDataAsset.h"
#include "MobilitySkillData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UMobilitySkillData : public USkillDataAsset
{
	GENERATED_BODY()
	
protected:
	virtual void Activate(UActiveSkillSlot* InActiveSkillSlot) override;
};
