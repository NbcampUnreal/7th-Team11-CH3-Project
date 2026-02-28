// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "Components/Items/WeaponItemDataAsset.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


void AWeaponActor::Init(UWeaponItemDataAsset* WeaponItem, USkeletalMeshComponent* SkeletalMesh)
{
	if (!WeaponItem)
	{
		return;
	}
	WeaponItemData = WeaponItem;
	DefaultSkill = WeaponItem->GetDefaultSkill();
	GripAnimation.LoadSynchronous();
}


float AWeaponActor::GetAttackRange() const
{
	return WeaponItemData->GetAttackRange();
}

UAnimSequence* AWeaponActor::GetGripAnimation() const
{
	return GripAnimation.Get();
}


EWeaponType AWeaponActor::GetWeaponType() const
{
	return WeaponItemData->GetWeaponType();
}

USkillDataAsset* AWeaponActor::GetDefaultSkillData() const
{
	return DefaultSkill;
}

