// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"


class UStatComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API AMonsterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMonsterBase();

protected:
	virtual void BeginPlay() override;
	FVector OriginLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

public:
	FVector GetOriginLocation() const;
	float GetAttackRange() const;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	FGenericTeamId TeamID;

};
