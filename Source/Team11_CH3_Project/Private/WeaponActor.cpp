// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "NaniteSceneProxy.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}


void AWeaponActor::Init(const FWeaponItemData& WeaponItem, USkeletalMeshComponent* SkeletalMesh)
{
	AttackMontage.LoadSynchronous();
	WeaponItemData = WeaponItem;
}

UAnimMontage* AWeaponActor::GetAttackMontage() const
{
	return AttackMontage.Get();
}

float AWeaponActor::GetAttackRange() const
{
	return AttackRange;
}



EWeaponType AWeaponActor::GetWeaponType() const
{
	return WeaponItemData.WeaponType;
}

