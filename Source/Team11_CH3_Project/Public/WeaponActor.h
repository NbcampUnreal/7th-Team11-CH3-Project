// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "GameFramework/Actor.h"
#include "Types/StatTypes.h"
#include "Components/Skills/SkillDataAsset.h"
#include "WeaponActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TEAM11_CH3_PROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();
	virtual void Init(const FWeaponItemData* WeaponItem, USkeletalMeshComponent* SkeletalMesh);
	float GetAttackRange() const;
	UFUNCTION(BlueprintPure)
	UAnimSequence* GetGripAnimation() const;

	virtual void StartAttack(const FVector& TargetLocation, USkillDataAsset* Skill)
	{
		CurrentSkillData = Skill;
		bIsAttacking = true;
	}

	virtual void PerformDamage()
	{
	}

	virtual void EndAttack()
	{
		CurrentSkillData.Reset();
		CurrentSkillData = nullptr;
		bIsAttacking = false;
	}

	EWeaponType GetWeaponType() const;
	USkillDataAsset* GetDefaultSkillData() const;
	bool IsAttacking() const { return bIsAttacking; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<USkillDataAsset> DefaultSkill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSoftObjectPtr<UAnimSequence> GripAnimation;
	UPROPERTY(VisibleInstanceOnly, Category="Weapon")
	TWeakObjectPtr<USkillDataAsset> CurrentSkillData;
	bool bIsAttacking;

private:
	FWeaponItemData WeaponItemData;
};
