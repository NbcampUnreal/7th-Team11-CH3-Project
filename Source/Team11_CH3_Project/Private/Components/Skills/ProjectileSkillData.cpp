// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ProjectileSkillData.h"
#include "Camera/CameraComponent.h"
#include "Components/Skills/BaseProjectile.h"

void UProjectileSkillData::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin, const FVector& TargetLocation)
{
	// 캐릭터 불러오기 수정 전 주석의 코드는 BasicAttack(깊이 2)과 다른 Skill들(깊이 3)과의 계층 깊이가 달라
	// 동일하게 적용했더니 Fireball의 경우엔 nullptr을 가르켰음
	//AActor* Owner = Cast<AActor>(GetOuter()->GetOuter());

	// StatComp 불러오기
	if (!IsValid(Instigator))
	{
		return;
	}
	UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
	FVector Direction = TargetLocation - Origin;
	// 손 -> 타겟 방향 계산
	FRotator SpawnRotation = Direction.Rotation();
	// 투사체 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Instigator;

	SpawnParams.Instigator = Cast<APawn>(Instigator);
	UWorld* W = Instigator->GetWorld();
	ABaseProjectile* Projectile = W->SpawnActor<ABaseProjectile>(
		ProjectileClass,
		Origin,
		SpawnRotation,
		SpawnParams
	);

	// Damage & ProjectileSpeed 계산
	float ActualDamage = Damage;
	if (IsValid(StatComp))
	{
		ActualDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
	}
	
	float ActualProjectileSpeed = ProjectileSpeed; 
	if (IsValid(StatComp))
	{
		ActualProjectileSpeed += StatComp->GetCurrentStat(EStat::ProjectileSpeed);
	}
	
	if (IsValid(Projectile))
	{
		Projectile->Initialize(ActualDamage, ActualProjectileSpeed);
	}
}

float UProjectileSkillData::GetScore(const AActor* Actor, const AActor* Target) const
{
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Actor);
	if (!IsValid(Actor)||!IsValid(Target))
	{
		return -1.0f;
	}
	Actor->GetWorld()->LineTraceSingleByChannel(Hit,Actor->GetActorLocation(),Target->GetActorLocation(),ECollisionChannel::ECC_WorldStatic,CollisionParams);

	if (Hit.bBlockingHit)
	{
		if (Hit.GetActor() == Target)
		{
			return 100.0f;
		}
	}
	
	return -1.0f;
}



void UProjectileSkillData::Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
	const FVector& TargetLocation, FName Name)
{
	Super::Notify(Instigator, WeaponActor, Origin, TargetLocation, Name);
	if (Name == TEXT("DealDamage"))
	{
		Activate(Instigator,WeaponActor,Origin,TargetLocation);
	}
	
}
