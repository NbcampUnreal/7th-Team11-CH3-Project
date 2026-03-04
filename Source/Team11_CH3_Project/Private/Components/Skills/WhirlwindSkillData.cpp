// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/WhirlwindSkillData.h"

#include "GenericTeamAgentInterface.h"
#include "Components/StatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelInstance/LevelInstanceTypes.h"

void UWhirlwindSkillData::DealDamage()
{
	APawn* Instigator = Cast<APawn> (ActiveSkillSlot->GetOwner());
	if (!Instigator)
	{
		return;
	}
	
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // Pawn만 검출

	UKismetSystemLibrary::SphereOverlapActors(
		Instigator->GetWorld(),
		Instigator->GetActorLocation(), 
		200.0f, 
		ObjectTypes, 
		APawn::StaticClass(),
		TArray<AActor*>(), 
		OverlappingActors
	);

	UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
	float BaseDamage = Damage;
	if (IsValid(StatComp))
	{
		BaseDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
	}
	
	
	for (AActor* HitActor : OverlappingActors)
	{
		if (IsValid(HitActor) == false || HitActor == Instigator)
		{
			continue;
		}
		AController* InstigatorController = Instigator->GetController();
		if (InstigatorController == nullptr)
		{
			continue;
		}

		if (const IGenericTeamAgentInterface* InstigatorTeam = Cast<IGenericTeamAgentInterface>(InstigatorController))
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
			Instigator,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("Hit : %s, Damage : %0.1f"), *HitActor->GetName(), ActualDamage);
	}
	
}


void UWhirlwindSkillData::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Timer += DeltaSeconds;
	AActor* Owner = ActiveSkillSlot->GetOwner();
	if (Timer > Duration)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			if (USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
			{
				if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
				{
					AnimInstance->Montage_Stop(0.2f, SkillMontage);
				}
			}
		}
		return;
	}
	FVector CurrentLoc = Owner->GetActorLocation();
	FVector Dir = (ActiveSkillSlot->GetTargetLocation() - CurrentLoc).GetSafeNormal();
	FVector NextLoc = CurrentLoc + (Dir * MoveSpeed* DeltaSeconds) ;
		
	FHitResult Hit;
	Owner->SetActorLocation(NextLoc, true, &Hit);
	
	
}

void UWhirlwindSkillData::OnExit()
{
	Super::OnExit();
	
}

float UWhirlwindSkillData::GetScore(const AActor* Actor, const AActor* Target) const
{
	if (FVector::DistSquared(Actor->GetActorLocation(), Target->GetActorLocation()) < Range * Range)
	{
		return 120.0f;
	}
	return -1.0f;
}

void UWhirlwindSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	const FVector& TargetLocation, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, TargetLocation, Name);
	if (Name == TEXT("DealDamage"))
	{
		DealDamage();
	}
}
