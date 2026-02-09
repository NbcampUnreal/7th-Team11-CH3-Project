// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipments.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AEquipments : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquipments();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
