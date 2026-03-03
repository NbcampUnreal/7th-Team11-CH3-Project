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
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
						  const FVector& TargetLocation) override;
	virtual void Enter(AActor* Actor, const FVector& TargetLocation) override;
	virtual void Execute() override;
	virtual void Tick(float DeltaSeconds, AActor* Actor, UActiveSkillSlot* ActiveSkillSlot) override;
	virtual void OnExit() override;
	
	virtual float GetScore(const AActor* Actor, const AActor* Target) const override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Dash")
	float ChargingTime = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Dash")
	float DashSpeed;

private:
	float ChargingTimer = 0.0f;
	bool bIsDashing = false;
	FVector NavDestination;
	FVector DashingDir;
};

