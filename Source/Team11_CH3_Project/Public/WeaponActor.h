// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "GameFramework/Actor.h"
#include "Types/StatTypes.h"
#include "WeaponActor.generated.h"

class USkillDataAsset;

UCLASS()
class TEAM11_CH3_PROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();
	virtual void Init(const FWeaponItemData& WeaponItem, USkeletalMeshComponent* SkeletalMesh);
	UAnimMontage* GetAttackMontage() const;
	float GetAttackRange() const;
	//TODO
	virtual void StartAttack(const FVector& Direction,  USkillDataAsset* Skill) {CurrentSkillData = Skill;}
	virtual void PerformDamage() {}
	virtual void EndAttack() {CurrentSkillData.Reset(); CurrentSkillData = nullptr;}
	EWeaponType GetWeaponType() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSoftObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TWeakObjectPtr<USkillDataAsset> CurrentSkillData;
private:
	FWeaponItemData WeaponItemData;

};
