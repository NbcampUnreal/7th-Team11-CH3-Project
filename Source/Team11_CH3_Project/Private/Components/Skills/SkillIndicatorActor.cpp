// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillIndicatorActor.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ASkillIndicatorActor::ASkillIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	RootComponent = NiagaraComponent;
}

void ASkillIndicatorActor::Initialize(APawn* InInstigator, float InMaxRange)
{
    OwnerInstigator = InInstigator;
	MaxRange = InMaxRange;

    if (IsValid(IndicatorEffect) == false)
        return;
    if (IsValid(NiagaraComponent) == false)
        return;

    NiagaraComponent->SetAsset(IndicatorEffect);
}


