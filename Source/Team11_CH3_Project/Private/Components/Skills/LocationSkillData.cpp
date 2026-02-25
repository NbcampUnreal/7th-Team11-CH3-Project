// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/LocationSkillData.h"
#include "Components/Skills/SkillIndicatorActor.h"

void ULocationSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction) const
{
	if (IsValid(Instigator) == false)
		return;
	if (IsValid(IndicatorClass) == false)
		return;
	if (bIsSelectingLocation)
		return;

	ULocationSkillData* MutableThis = const_cast<ULocationSkillData*>(this);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Instigator;

	ASkillIndicatorActor* Indicator = Instigator->GetWorld()->SpawnActor<ASkillIndicatorActor>(
		IndicatorClass,
		Instigator->GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (IsValid(Indicator) == false)
		return;

	Indicator->Initialize(Instigator, MaxRange);
	MutableThis->SpawnedIndicator = Indicator;
	MutableThis->bIsSelectingLocation = true;
}

void ULocationSkillData::Confirm()
{
}

void ULocationSkillData::Cancel()
{
}
