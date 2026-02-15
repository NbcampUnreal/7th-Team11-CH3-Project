// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMonsterData.h"
#include "GenericTeamAgentInterface.h"
#include "MonsterControllerBase.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"


class AWeaponActor;
class UStatComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMonsterBase();

	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	bool TryAttack(AActor* Target);
	
	void DealDamage();
	void BlackboardUpdate();
	void Init(const FMonsterData& MonsterData);
	void Clear();
	FOnAttackFinished OnAttackFinished;
protected:
	
	FVector OriginLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeaponActor> WeaponActor;
	
public:
	FVector GetOriginLocation() const;
	float GetAttackRange() const;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bIsAttacking;
};
