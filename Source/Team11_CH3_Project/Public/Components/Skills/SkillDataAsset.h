// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkillSlot.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "SkillDataAsset.generated.h"


class AWeaponActor;
class ABaseSkill;
/**
 * 
 */
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Immediately,
	Aiming,
	Duration
};

UCLASS()
class TEAM11_CH3_PROJECT_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void Activate(UActiveSkillSlot* InActiveSkillSlot){ActiveSkillSlot = InActiveSkillSlot;}
	virtual void Execute() {}
	virtual void Tick(float DeltaSeconds) {}
	virtual void OnExit() {}
	
	
	
	UTexture2D* GetThumbnail()const{return Thumbnail.LoadSynchronous();}
	FText GetSKillName()const { return SkillName; }
	float GetCooldownTime() const { return CooldownTime; }
	virtual float GetScore(const AActor* Actor, const AActor* Target) const { return -1.0f; }
	UAnimMontage* GetSkillMontage() const { return SkillMontage; }


	ESkillType GetSkillType() const { return SkillType; }
	UNiagaraSystem* GetMagicCircleEffect() const { return MagicCircleEffect; }
	
	float GetRange() const { return Range; }
	float GetDuration()const{return Duration;}
	virtual void Notify(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
						  const FVector& TargetLocation, FName Name){};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Thumbnail;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName SkillID;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float CooldownTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Range = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Duration = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SkillMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	ESkillType SkillType;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UNiagaraSystem> MagicCircleEffect;
protected:
	UPROPERTY()
	TWeakObjectPtr<UActiveSkillSlot> ActiveSkillSlot;
};
