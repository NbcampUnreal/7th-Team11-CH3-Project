// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataAsset.h"
#include "WhirlwindSkillData.generated.h"

class USoundBase;

/**
 * 
 */


UCLASS()
class TEAM11_CH3_PROJECT_API UWhirlwindSkillData : public USkillDataAsset
{
	GENERATED_BODY()
public:
	void DealDamage();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnExit() override;
	virtual float GetScore(const AActor* Actor, const AActor* Target) const override;
	virtual void Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation, FName Name)  override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Dash")
	float MoveSpeed = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Sound")
	USoundBase* WhirlwindSound;
private:
	float Timer = 0.0f;
	
};
