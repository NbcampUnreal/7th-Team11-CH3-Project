// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/ItemTypes.h"
#include "ItemManager.generated.h"


class UEquipmentInstance;
class UWeaponItemDataAsset;
class AWeaponActor;
class UEquipmentItemDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemManager();

	// 장비 해제
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Equip(UEquipmentInstance* Data);
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Unequip(EEquipmentType SlotType);
	void EquipWeapon(UEquipmentInstance* WeaponItemInstance);

	// GameInstance에서 장비 데이터 받아오는 함수
	void RestoreEquipment(TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> EquipmentData);
	// Getter
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWeaponActor* GetCurrentWeapon() const { return CurrentWeapon; }
	TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>>& GetEquipments() { return Equipments; }
	void Clear();

	// 아이템 장착 해제 시 스탯 수정
	void UnequipWeapon();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


	// 장착중인 무기
	UPROPERTY(EditDefaultsOnly, Category = "Item|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponActor> CurrentWeapon;
	// 장착 중인 무기 데이터
	// 부위별 장착 방어구 추적
	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> Equipments;
	// 버프 ID
	TMap<FName, TArray<int32>> EquipmentBuffIDs;
};
