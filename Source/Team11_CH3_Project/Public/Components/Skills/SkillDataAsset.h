// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

class AWeaponActor;
class ABaseSkill;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	                      const FVector& Direction) const
	{
	}

	float GetCooldownTime() const { return CooldownTime; }
	virtual float GetScore(AActor* Actor, AActor* Target) const{return -1.0f;}
	UAnimMontage* GetSkillMontage() const { return SkillMontage; };
	float GetRange() const { return Range; };
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName SkillID;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float CooldownTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Range = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SkillMontage = nullptr;
};
