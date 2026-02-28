// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "GreatSwordWeaponActor.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AGreatSwordWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGreatSwordWeaponActor();
	virtual void Init(UWeaponItemDataAsset* WeaponItemDataAsset, USkeletalMeshComponent* SkeletalMesh) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
private:
	FVector Direction;
};
