// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BuffSkillData.h"
#include "NiagaraFunctionLibrary.h"


void UBuffSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, TargetLocation, Name);
	if (Name == TEXT("DealDamage"))
	{
		DoBuff();
	}
}

void UBuffSkillData::DoBuff()
{
	APawn* Instigator = Cast<APawn>(ActiveSkillSlot->GetOwner());;
	
	if (IsValid(Instigator) == false)
		return;
	UBuffComponent* BuffManager = Instigator->FindComponentByClass<UBuffComponent>();

	if (IsValid(BuffManager) == false)
		return;

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

	BuffManager->AddBuff(TargetStat, BuffType, BuffAmount, BuffDuration);
	UE_LOG(LogTemp, Warning, TEXT("Buff : %s"), *Instigator->GetName());
}
