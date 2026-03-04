// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataAsset.h"
#include "DashSkill.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UDashSkill : public USkillDataAsset
{
	GENERATED_BODY()
public:
	virtual void Activate(UActiveSkillSlot* InActiveSkillSlot) override;
	virtual void Execute() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnExit() override;
	
	virtual float GetScore(const AActor* Actor, const AActor* Target) const override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Dash")
	float ChargingTime = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Dash")
	float DashSpeed;

private:
	float ChargingTimer;
	bool bIsDashing;
	FVector NavDestination;
	FVector DashingDir;
	bool bIsHit;
};

