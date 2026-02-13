// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "NaniteSceneProxy.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponActor::Init(FWeaponItemData WeaponItem)
{
	AttackMontage.LoadSynchronous();
	//TODO
	//WeaponItem.Damage;
	if (auto* AttackDamage = WeaponItem.StatBonuses.Find(EStat::AttackDamage))
	{
		Damage = *AttackDamage;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackDamage not found in StatBonuses"));
	}
	
}

UAnimMontage* AWeaponActor::GetAttackMontage() const
{
	return AttackMontage.Get();
}

float AWeaponActor::GetAttackRange() const
{
	return AttackRange;
}


