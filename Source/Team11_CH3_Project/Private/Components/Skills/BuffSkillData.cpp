// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BuffSkillData.h"


void UBuffSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation) 
{
	if (IsValid(Instigator) == false)
		return;
	UBuffManager* BuffManager = Instigator->FindComponentByClass<UBuffManager>();

	if (IsValid(BuffManager) == false)
		return;

	BuffManager->AddBuff(TargetStat, BuffType, BuffAmount, BuffDuration);
	UE_LOG(LogTemp, Warning, TEXT("Buff : %s"), *Instigator->GetName());
}

void UBuffSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, Direction, Name);
	if (Name == TEXT("DealDamage"))
	{
		Activate(Instigator, WeaponActor, Origin, Direction);
	}
}
