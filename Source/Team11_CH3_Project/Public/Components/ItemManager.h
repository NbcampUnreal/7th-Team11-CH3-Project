// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/ItemTypes.h"
#include "ItemManager.generated.h"


class UGemItemDataAsset;
class UEquipmentSlot;
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
	virtual void InitializeComponent() override;
	// 장비 해제
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Equip(UEquipmentInstance* Data);
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Unequip(EEquipmentType SlotType);
	void EquipWeapon(UEquipmentInstance* WeaponItemInstance);

	UFUNCTION(BlueprintCallable, Category = "Item|Gem")
	void EquipSkillGem(UEquipmentInstance* SkillGemInstance, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Item|Gem")
	void UnEquipSkillGem(int32 Index);
	
	// GameInstance에서 장비 데이터 받아오는 함수
	void RestoreEquipment(TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> EquipmentData);
	// Getter
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWeaponActor* GetCurrentWeapon() const { return CurrentWeapon; }
	TMap<EEquipmentType, TObjectPtr<UEquipmentInstance>> GetEquipments();
	void Clear();

	// 아이템 장착 해제 시 스탯 수정
	void UnequipWeapon();
	// 장비 Stat 변경시 사용
	UFUNCTION()
	void OnEquipmentStatChanged(EEquipmentType Type, UEquipmentInstance* Instance);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


	// 장착중인 무기
	UPROPERTY(EditDefaultsOnly, Category = "Item|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponActor> CurrentWeapon;
	// 장착 중인 장비 데이터
	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentType,TObjectPtr<UEquipmentSlot>> EquipmentSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UEquipmentSlot>> GemSlots;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 MaxSKillGemCount = 3;
	// 버프 ID
	TMap<EEquipmentType, TArray<int32>> EquipmentBuffIDs;
};
