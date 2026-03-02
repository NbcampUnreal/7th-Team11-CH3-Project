// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/ParticleDamageActor.h"
#include "NiagaraComponent.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"

// Sets default values
AParticleDamageActor::AParticleDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	RootComponent = NiagaraComponent;
}

void AParticleDamageActor::Initialize(float InDamage)
{
	Damage = InDamage;
}

// Called when the game starts or when spawned
void AParticleDamageActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);

	if (IsValid(NiagaraComponent) == false)
		return;
	NiagaraComponent->SetVariableObject(FName("CallbackHandler"), this);
}

void AParticleDamageActor::ReceiveParticleData_Implementation(
	const TArray<FBasicParticleData>& Data, 
	UNiagaraSystem* OwningSystem, 
	const FVector& SimulationPositionOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("ReceiveParticleData called, count: %d"), Data.Num());

	for (const FBasicParticleData& Particle : Data)
	{
		// 파티클에 충돌한 위치 데이터 -> DeaDamageAtLocation으로 전달
		FVector ImpactLocation = Particle.Position + SimulationPositionOffset;
		DealDamageAtLocation(ImpactLocation);
	}
}

void AParticleDamageActor::DealDamageAtLocation(const FVector& ImpactLocation)
{
	APawn* InstigatorPawn = GetInstigator();

	if (IsValid(InstigatorPawn) == false)
		return;

	AController* InstigatorController = InstigatorPawn->GetController();

	// 데미지 계산
	float FinalDamage = Damage;
	UStatComponent* StatComp = InstigatorPawn->FindComponentByClass<UStatComponent>();
	if (IsValid(StatComp) == false)
		return;
	
	float CritChance = StatComp->GetCurrentStat(EStat::CriticalChance);
	float CritMultiPlier = StatComp->GetCurrentStat(EStat::CriticalDamage);
	if (FMath::FRandRange(0.f, 100.f) < CritChance)
	{
		FinalDamage *= CritMultiPlier;
	}

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		ImpactLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DamageRadius),
		QueryParams
	);

	TSet<AActor*> DamagedActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (Overlap.Component.IsValid() == false)
			continue;

		AActor* HitActor = Overlap.GetActor();
		if (IsValid(HitActor) == false)
			continue;

		if (DamagedActors.Contains(HitActor))
			continue;

		DamagedActors.Add(HitActor);

		UGameplayStatics::ApplyDamage(
			HitActor,
			FinalDamage,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage to: %s"), *GetNameSafe(HitActor));
	}


}


