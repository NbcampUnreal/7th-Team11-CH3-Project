// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ExplosionSkillActor.h"
#include "Components/SphereComponent.h"
#include "Components/StatComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

// Sets default values
AExplosionSkillActor::AExplosionSkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetSphereRadius(300.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AExplosionSkillActor::Initialize(float InDamage)
{
	Damage = InDamage;
	DealAreaDamage();
}

// Called when the game starts or when spawned
void AExplosionSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetSphereRadius(DamageRadius);
	SetLifeSpan(LifeTime);
}

void AExplosionSkillActor::DealAreaDamage()
{
	// 데미지 주체 가져오기
	APawn* InstigatorPawn = GetInstigator();
	if (IsValid(InstigatorPawn) == false)
		return;

	AController* InstigarorController = InstigatorPawn->GetController();
	// 데미지 계산
	float FinalDamage = Damage;
	UStatComponent* StatComp = InstigatorPawn->FindComponentByClass<UStatComponent>();
	if (IsValid(StatComp) == false)
		return;
	// 크리티컬 데미지 계산
	float CritChance = StatComp->GetCurrentStat(EStat::CriticalChance);
	float CritMultiplier = StatComp->GetCurrentStat(EStat::CriticalDamage);
	if (FMath::FRandRange(0.f, 100.f) < CritChance)
	{
		FinalDamage *= CritMultiplier;
	}
	// OverlapMultiByChannel이용해서 겹치는 모든 액터 데미지
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;

	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DamageRadius),
		QueryParams
	);
	// 오버랩된 모든 액터 ApplyDamage
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();
		if (IsValid(HitActor) == false)
			continue;

		UGameplayStatics::ApplyDamage(
			HitActor,
			FinalDamage,
			InstigarorController,
			this,
			UDamageType::StaticClass()
		);
	}
}


