// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Skills/BaseSkill.h"
#include "ProjectileSkillData.generated.h"

class ABaseProjectile;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UProjectileSkillData : public USkillDataAsset
{
	GENERATED_BODY()
	
protected:

	virtual void Activate(APawn* Instigator, const FVector& Origin, const FVector& Direction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;

};
