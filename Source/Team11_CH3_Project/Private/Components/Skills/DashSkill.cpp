// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/DashSkill.h"

void UDashSkill::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	const FVector& Direction) const
{
	Super::Activate(Instigator, WeaponActor, Origin, Direction);
}

float UDashSkill::GetScore(AActor* Actor, AActor* Target) const
{
	return Super::GetScore(Actor, Target);
}
