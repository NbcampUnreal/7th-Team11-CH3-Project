// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor.h"

AInteractionActor::AInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

