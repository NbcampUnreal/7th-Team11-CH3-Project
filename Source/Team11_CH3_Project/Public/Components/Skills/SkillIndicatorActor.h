// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillIndicatorActor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API ASkillIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillIndicatorActor();

	virtual void Tick(float DeltaTime) override;

	void Initialize(APawn* InInstigator, float InMaxRange);

	FVector GetIndicatorLocation() const { return IndicatorLocation; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Indicator")
	TObjectPtr<UNiagaraSystem> IndicatorEffect;

private:	
	void UpdateLocation();

	UPROPERTY()
	TWeakObjectPtr<APawn> OwnerInstigator;
	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	FVector IndicatorLocation;
	float MaxRange = 1500.f;
};
