// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/LocationSkillData.h"
#include "Components/Skills/SkillIndicatorActor.h"

void ULocationSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& Direction) const
{
}

void ULocationSkillData::Enter() 
{
}

void ULocationSkillData::Execute() 
{

	if (IsValid(SpawnedIndicator) == false)
		return;
	FVector SkillLocation = SpawnedIndicator->GetIndicatorLocation();

	if (IsValid(SkillEffectClass) == false)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Effect = SpawnedIndicator->GetWorld()->SpawnActor<AActor>(
		SkillEffectClass,
		SkillLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	UE_LOG(LogTemp, Warning, TEXT("Spawned Effect: %s at %s"),
		*GetNameSafe(Effect), *SkillLocation.ToString());

	SpawnedIndicator->Destroy();
	SpawnedIndicator = nullptr;
}

void ULocationSkillData::Tick(float DeltaSeconds, AActor* Actor) 
{
	if (IsValid(SpawnedIndicator) || IsValid(Actor) == false || IsValid(IndicatorClass) == false)
		return;

	APawn* Instigator = Cast<APawn>(Actor);
	if (IsValid(Instigator) == false)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Actor;

	ASkillIndicatorActor* Indicator = Actor->GetWorld()->SpawnActor<ASkillIndicatorActor>(
		IndicatorClass,
		Actor->GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (IsValid(Indicator) == false)
		return;

	Indicator->Initialize(Instigator, Range);
	SpawnedIndicator = Indicator;
}

void ULocationSkillData::OnExit() 
{
	if (IsValid(SpawnedIndicator) == false)
		return;

	SpawnedIndicator->Destroy();
	SpawnedIndicator = nullptr;
	
}
