// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionSkillActor.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class TEAM11_CH3_PROJECT_API AExplosionSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosionSkillActor();
	UFUNCTION()
	void Initialize(float InDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UFUNCTION()
	void DealAreaDamage();

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	float DamageRadius = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	float LifeTime = 3.f;

	float Damage = 0.f;
};
