// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "StaffWeaponActor.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AStaffWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaffWeaponActor();
	virtual void Init(const FWeaponItemData& WeaponItem, USkeletalMeshComponent* SkeletalMesh) override;
	virtual void StartAttack(AActor* Target) override;
	virtual void PerformDamage() override;
	virtual void EndAttack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
