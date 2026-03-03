// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/MeleeSimpleAttackSkillData.h"

#include "GenericTeamAgentInterface.h"
#include "MeshPassProcessor.inl"
#include "WeaponActor.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"

void UMeleeSimpleAttackSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation) 
{
	// StatComp 불러오기
	if (!IsValid(Instigator) || !IsValid(WeaponActor))
	{
		return;
	}
	UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
	

	// Damage & ProjectileSpeed 계산
	float BaseDamage = Damage;
	if (IsValid(StatComp))
	{
		BaseDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
	}

	TArray<AActor*> HitActors;
	WeaponActor->GetOverlappingActors(HitActors);

	for (AActor* HitActor : HitActors)
	{
		if (IsValid(HitActor) == false || HitActor == Instigator || HitActor == WeaponActor)
		{
			continue;
		}
		if (IsValid(Instigator) == false)
		{
			continue;
		}
		AController* InstigatorController = Instigator->GetController();
		if (InstigatorController == nullptr)
		{
			continue;
		}
		
		if (const IGenericTeamAgentInterface* InstigatorTeam =  Cast<IGenericTeamAgentInterface>(InstigatorController))
		{
			if (InstigatorTeam->GetTeamAttitudeTowards(*HitActor) != ETeamAttitude::Hostile)
			{
				continue;
			}
		}

		float ActualDamage = BaseDamage;
		if (IsValid(StatComp))
		{
			// 크리티컬 판정
			float CriticalChance = StatComp->GetCurrentStat(EStat::CriticalChance);
			float CriticalMultiplier = StatComp->GetCurrentStat(EStat::CriticalDamage);

			if (FMath::FRandRange(0.0f, 100.0f) < CriticalChance)
			{
				ActualDamage *= CriticalMultiplier;
				UE_LOG(LogTemp, Warning, TEXT("Critical Hit"));
			}
		}
		UGameplayStatics::ApplyDamage(
			HitActor,
			ActualDamage,
			InstigatorController,
			WeaponActor,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("Hit : %s, Damage : %0.1f"), *HitActor->GetName(), ActualDamage);

	}
	
	// 사운드 출력
	if (SwordSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordSound, Instigator->GetActorLocation(), 0.5f);
	}
	
	
	
}

float UMeleeSimpleAttackSkillData::GetScore(const AActor* Actor, const AActor* Target) const
{
	if (FVector::DistSquared(Actor->GetActorLocation(), Target->GetActorLocation()) < Range*Range)
	{
		return 100.0f;
	}
	return -1.0f;
}

void UMeleeSimpleAttackSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	const FVector& TargetLocation, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, TargetLocation, Name);
	if (Name == TEXT("DealDamage"))
	{
		Activate(Instigator,WeaponActor,Origin,TargetLocation);
	}
}
