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
	virtual void Enter() const override;
	virtual void Execute() const override;
	virtual void Tick(float DeltaSeconds, AActor* Actor) const override;
	virtual void OnExit() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<ASkillIndicatorActor> IndicatorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<AActor> SkillEffectClass;


private:

	UPROPERTY()
	mutable TObjectPtr<ASkillIndicatorActor> SpawnedIndicator;
};
