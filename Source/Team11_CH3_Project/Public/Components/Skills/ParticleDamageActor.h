// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "ParticleDamageActor.generated.h"

class UNiagaraComponent;
class UStatComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API AParticleDamageActor : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AParticleDamageActor();
	UFUNCTION()
	void Initialize(float InDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ReceiveParticleData_Implementation(
		const TArray<FBasicParticleData>& Data,
		UNiagaraSystem* OwningSystem,
		const FVector& SimulationPositionOffset) override;
private:
	UFUNCTION()
	void DealDamageAtLocation(const FVector& ImpactLocation);

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleDamage", meta = (AllowPrivateAccess = "true"))
	float DamageRadius = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleDamage", meta = (AllowPrivateAccess = "true"))
	float LifeTime = 5.f;

	float Damage = 0.f;
};
