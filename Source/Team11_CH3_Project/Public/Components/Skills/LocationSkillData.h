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
	virtual void Activate(UActiveSkillSlot* InActiveSkillSlot) override;
	virtual void Execute()  override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnExit()  override;
	// 공격용 액터 스폰
	void SpawnSkill();
	virtual float GetScore(const AActor* Actor, const AActor* Target) const override;
	
	virtual void Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction, FName Name) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<ASkillIndicatorActor> IndicatorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Location")
	TSubclassOf<AActor> SkillEffectClass;


private:

	UPROPERTY()
	mutable TObjectPtr<ASkillIndicatorActor> SpawnedIndicator;
	bool bIsExecuted = false;
	
};
