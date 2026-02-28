// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/StatTypes.h"
#include "Components/Skills/SkillDataAsset.h"
#include "WeaponActor.generated.h"


class UWeaponItemDataAsset;
class USkillSlot;

UCLASS(Blueprintable, BlueprintType)
class TEAM11_CH3_PROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();
	virtual void Init(UWeaponItemDataAsset* WeaponItem, USkeletalMeshComponent* SkeletalMesh);
	float GetAttackRange() const;
	UFUNCTION(BlueprintPure)
	UAnimSequence* GetGripAnimation() const;
	EWeaponType GetWeaponType() const;
	USkillDataAsset* GetDefaultSkillData() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<USkillDataAsset> DefaultSkill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSoftObjectPtr<UAnimSequence> GripAnimation;

private:
	TWeakObjectPtr<UWeaponItemDataAsset> WeaponItemData;
};
