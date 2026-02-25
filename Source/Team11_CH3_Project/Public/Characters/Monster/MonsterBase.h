// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMonsterData.h"
#include "GenericTeamAgentInterface.h"
#include "MonsterControllerBase.h"
#include "Components/Skills/SkillSlot.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"


class USkillManager;
class AWeaponActor;
class UStatComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMonsterBase();
	void EquipWeapon(FWeaponItemData* WeaponItemData);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	void OnDieMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	virtual bool TryAttack(AActor* Target);
	void PerformAttack(USkillSlot* SkillSlot, const FVector& TargetLocation);
	
	void DealDamage();
	void BlackboardUpdate();
	void Init(const FMonsterData* MonsterData);
	void Clear();
	// Getter
	int32 GetScoreValue() const { return ScoreValue; }
	FOnAttackFinished OnAttackFinished;
	
protected:
	
	FVector OriginLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillManager> SkillComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeaponActor> WeaponActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MonsterDieAnimMontage;

	int32 ScoreValue = 0;
	
public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;
	FVector GetOriginLocation() const;
	float GetAttackRange() const;
	UStatComponent* GetStatComponent() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
