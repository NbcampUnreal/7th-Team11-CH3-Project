// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Items/ItemDataBase.h"
#include "Components/Items/PotionItemData.h"
#include "Components/Items/SkillGemItemData.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "Components/Items/Equipments/ArmorItemData.h"
#include "ItemManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemManager();

	// 아이템 사용(타입별 분기)
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem(FName RowName, EItemType ItemType, int32 SlotIndex = -1);

	// 장비 해제
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipWeapon();
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipArmor(EEquipmentType SlotType);

	// GameInstance에서 장비 데이터 받아오는 함수
	UFUNCTION(BlueprintCallable, Category = "Item")
	void RestoreEquipment(const FWeaponItemData& WeaponData, const TMap<EEquipmentType, FArmorItemData>& ArmorData);

	// Getter
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWeaponActor* GetCurrentWeapon() const { return CurrentWeapon; }
	const FWeaponItemData& GetCachedWeaponData() const { return CachedWeaponData; }
	const TMap<EEquipmentType, FArmorItemData>& GetEquippedArmors() const { return EquippedArmors; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// ItemType에 따른 사용 로직
	void UsePotion(FPotionItemData* Data);
	void UseSkillGem(FSkillGemItemData* Data, int32 SlotIndex);
	void EquipWeapon(FWeaponItemData* Data);
	void EquipArmor(FArmorItemData* Data);
	// 아이템 장착 해제 시 스탯 수정
	void ApplyStatBonuses(FName ItemRowName, TMap<EStat, float>& StatBonuses);
	void RemoveStatBonuses(FName ItemID);

	// 장착중인 무기
	UPROPERTY(EditDefaultsOnly, Category = "Item|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponActor> CurrentWeapon;
	// 장착 중인 무기 데이터
	UPROPERTY()
	FWeaponItemData CachedWeaponData;
	// 부위별 장착 방어구 추적
	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentType, FArmorItemData> EquippedArmors;
	// 버프 ID
	TMap<FName, TArray<int32>> EquipmentBuffIDs;
};
