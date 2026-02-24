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
	
public:
	virtual void Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction) const override;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;

};
