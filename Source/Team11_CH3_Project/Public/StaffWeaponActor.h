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
	virtual void Init(UWeaponItemDataAsset* WeaponItem, USkeletalMeshComponent* SkeletalMesh) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
private:
	FVector Direction;
};
