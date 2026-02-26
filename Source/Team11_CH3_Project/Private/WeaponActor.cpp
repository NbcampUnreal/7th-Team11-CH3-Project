// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


void AWeaponActor::Init(const FWeaponItemData* WeaponItem, USkeletalMeshComponent* SkeletalMesh)
{
	if (!WeaponItem)
	{
		return;
	}
	WeaponItemData = *WeaponItem;
	DefaultSkill = WeaponItem->DefaultSkill.LoadSynchronous();
	GripAnimation.LoadSynchronous();
}


float AWeaponActor::GetAttackRange() const
{
	return WeaponItemData.AttackRange;
}

UAnimSequence* AWeaponActor::GetGripAnimation() const
{
	return GripAnimation.Get();
}


EWeaponType AWeaponActor::GetWeaponType() const
{
	return WeaponItemData.WeaponType;
}

USkillDataAsset* AWeaponActor::GetDefaultSkillData() const
{
	return DefaultSkill;
}

