// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/LocationSkillData.h"
#include "Components/Skills/SkillIndicatorActor.h"
#include "Components/Skills/ExplosionSkillActor.h"
#include "Components/Skills/ParticleDamageActor.h"
#include "Components/StatComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/Monster/MonsterBase.h"

void ULocationSkillData::Activate(UActiveSkillSlot* InActiveSkillSlot)
{
	Super::Activate(InActiveSkillSlot);

	UNiagaraSystem* MagicCircle = GetMagicCircleEffect();
	if (IsValid(MagicCircle) == false)
		return;
	AActor* Owner = InActiveSkillSlot->GetOwner();
	FVector SpawnLocation = Owner->GetActorLocation();
	SpawnLocation.Z -= 85.f;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Owner->GetWorld(),
		MagicCircle,
		SpawnLocation,
		Owner->GetActorRotation()
	);
}

void ULocationSkillData::Execute()
{
	if (bIsExecuted)
		return;
	bIsExecuted = true;

	if (IsValid(SpawnedIndicator) == false)
		return;
	ACharacter* Character = Cast<ACharacter>(SpawnedIndicator->GetOwner());
	if (IsValid(Character) == false)
		return;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
		return;

	AnimInstance->Montage_JumpToSection("EndCasting", SkillMontage);
}

void ULocationSkillData::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AActor* Owner = ActiveSkillSlot->GetOwner();
	if (IsValid(SpawnedIndicator) || IsValid(Owner) == false || IsValid(IndicatorClass) == false)
		return;

	APawn* Instigator = Cast<APawn>(Owner);
	if (IsValid(Instigator) == false)
		return;

	FVector SpawnLocation = Owner->GetActorLocation();

	SpawnLocation.Z -= 85.f; // 발밑으로 보정

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;

	ASkillIndicatorActor* Indicator = Owner->GetWorld()->SpawnActor<ASkillIndicatorActor>(
		IndicatorClass,
		SpawnLocation,
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
	bIsExecuted = false;
	if (IsValid(SpawnedIndicator) == false)
		return;

	SpawnedIndicator->Destroy();
	SpawnedIndicator = nullptr;
}

void ULocationSkillData::SpawnSkill()
{
	if (IsValid(SpawnedIndicator) == false)
		return;

	FVector SkillLocation = SpawnedIndicator->GetIndicatorLocation();
	if (ActiveSkillSlot->GetTarget() != nullptr)
	{
		FVector TargetLocation = ActiveSkillSlot->GetTargetLocation();
		FVector End = TargetLocation - FVector(0.0f, 0.0f, 1000.0f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActiveSkillSlot->GetTarget());
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TargetLocation,
			End,
			ECC_WorldStatic, // 주로 바닥(Static Mesh)은 이 채널을 사용합니다.
			QueryParams
		);
		if (bHit)
		{
			SkillLocation = HitResult.ImpactPoint; 
		}
	}

	SkillLocation.Z += 5;
	if (IsValid(SkillEffectClass) == false)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = SpawnedIndicator->GetOwner();
	SpawnParams.Instigator = Cast<APawn>(SpawnedIndicator->GetOwner());

	AActor* Effect = SpawnedIndicator->GetWorld()->SpawnActor<AActor>(
		SkillEffectClass,
		SkillLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
	// ExlosionSkillActor면 데미지 초기화
	if (AExplosionSkillActor* Explosion = Cast<AExplosionSkillActor>(Effect))
	{
		float ActualDamage = Damage;
		UStatComponent* StatComp = SpawnParams.Instigator->FindComponentByClass<UStatComponent>();
		if (IsValid(StatComp))
		{
			ActualDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
			Explosion->Initialize(ActualDamage);
		}
	}

	if (AParticleDamageActor* ParticleActor = Cast<AParticleDamageActor>(Effect))
	{
		float ActualDamage = Damage;
		UStatComponent* StatComp = SpawnParams.Instigator->FindComponentByClass<UStatComponent>();
		if (IsValid(StatComp))
			ActualDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
		ParticleActor->Initialize(ActualDamage);
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawned Effect: %s at %s"),
	       *GetNameSafe(Effect), *SkillLocation.ToString());

	SpawnedIndicator->Destroy();
	SpawnedIndicator = nullptr;
}

float ULocationSkillData::GetScore(const AActor* Actor, const AActor* Target) const
{
	if (FVector::DistSquared(Target->GetActorLocation(), Actor->GetActorLocation()) < Range * Range)
	{
		return 100.0f;
	}
	return Super::GetScore(Actor, Target);
}

void ULocationSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
                                const FVector& Direction, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, Direction, Name);
	if (Name == TEXT("DealDamage"))
	{
		SpawnSkill();
	}
	else if (Name == TEXT("Cancel"))
	{
		ACharacter* Character = Cast<ACharacter>(Instigator);
		if (IsValid(Character) == false)
			return;
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance) == false)
			return;
		AnimInstance->Montage_Stop(0.2f, SkillMontage);
	}
}
