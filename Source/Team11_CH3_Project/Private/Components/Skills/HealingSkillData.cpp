// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/HealingSkillData.h"
#include "Components/StatComponent.h"
#include "NiagaraFunctionLibrary.h"

void UHealingSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation)
{
	if (IsValid(Instigator) == false)
		return;
	UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
	if (IsValid(StatComp) == false)
		return;

	// 마법진 소환
	UNiagaraSystem* MagicCircle = GetMagicCircleEffect();
	if (IsValid(MagicCircle) == false)
		return;

	FVector SpawnLocation = Instigator->GetActorLocation();
	SpawnLocation.Z -= 85.f;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Instigator->GetWorld(),
		MagicCircle,
		SpawnLocation,
		Instigator->GetActorRotation()
	);
	// HP 즉시 회복
	StatComp->AddCurrentHP(HealAmount);
}

void UHealingSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, TargetLocation, Name);
	if (Name == TEXT("DealDamage"))
	{
		Activate(Instigator, WeaponActor, Origin, TargetLocation);
	}
}
