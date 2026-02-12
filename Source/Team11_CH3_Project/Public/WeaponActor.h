// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();
	virtual void BeginPlay() override;
	void Init(FWeaponItemData WeaponItem);
	UAnimMontage* GetAttackMontage() const;
	float GetAttackRange()const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSoftObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
