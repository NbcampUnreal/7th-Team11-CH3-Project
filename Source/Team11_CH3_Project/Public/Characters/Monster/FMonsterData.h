// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "Components/StatComponent.h"
#include "FMonsterData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	FStatData StatData;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	float BaseExp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	TSoftObjectPtr<UTexture2D> Thumbnail;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	//UWeaponItem WeaponItemClass;
	
	//TODO WeaponItem 
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	// TSoftObjectPtr<UAnimMontage> AttackMontage;
	//
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	TSoftClassPtr<UAnimInstance> AnimBlueprint;
};
