// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "MeleeBossMonster.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AMeleeBossMonster : public AMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeBossMonster();

	virtual bool TryAttack(AActor* Target) override;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MeleeBossMonster|Attack")
	float MeleeRange;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MeleeBossMonster|Attack")
	float MiddleRange;
	
	
public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
