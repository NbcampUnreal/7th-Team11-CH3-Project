// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

// void AWeaponActor::Init(UWeaponItem WeaponItem)
// {
// 	AttackMontage.LoadSynchronous();
// }

UAnimMontage* AWeaponActor::GetAttackMontage() const
{
	return AttackMontage.Get();
}

float AWeaponActor::GetAttackRange() const
{
	return AttackRange;
}


