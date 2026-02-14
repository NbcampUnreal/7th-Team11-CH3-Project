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
	void UseItem(FName RowName, EItemType ItemType, int32 SlotIndex);

	// 장비 해제
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipArmor(EEquipmentType SlotType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// ItemTpye에 따른 사용 로직
	void UsePotion(FPotionItemData* Data);
	void UseSkillGem(FSkillGemItemData* Data, int32 SlotIndex);
	void EquipWeapon(FWeaponItemData* Data);
	void EquipArmor(FArmorItemData* Data);
	// 아이템 장착 해제 시 스탯 수정
	void ApplyStatBonuses(TMap<EStat, float> StatBonuses, bool bRemove);
	//데이터 테이블 참조
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> PotionTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> SkillGemTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ArmorTable;

	// 장착중인 무기
	UPROPERTY(EditDefaultsOnly, Category = "Item|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponActor> CurrentWeapon;
	// 장착 중인 무기 데이터
	UPROPERTY()
	FWeaponItemData CachedWeaponData;
	// 부위별 장착 방어구 추적
	UPROPERTY(EditDefaultsOnly, Category = "Item|Armor", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentType, FArmorItemData> EquippedArmors;
};
