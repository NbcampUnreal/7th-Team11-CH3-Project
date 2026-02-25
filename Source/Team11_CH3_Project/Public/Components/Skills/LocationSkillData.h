// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/SkillDataAsset.h"
#include "LocationSkillData.generated.h"

/**
 * 
 */
class ASkillIndicatorActor;

UCLASS()
class TEAM11_CH3_PROJECT_API ULocationSkillData : public USkillDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction) const override;

	void Confirm();

	void Cancel();

	bool IsSelectingLocation() const { return bIsSelectingLocation; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<ASkillIndicatorActor> IndicatorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<AActor> SkillEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	float MaxRange = 1500.f;


private:
	bool bIsSelectingLocation = false;

	UPROPERTY()
	TObjectPtr<ASkillIndicatorActor> SpawnedIndicator;
};
