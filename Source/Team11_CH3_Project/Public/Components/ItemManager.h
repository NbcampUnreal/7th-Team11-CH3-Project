// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "Components/ActorComponent.h"
#include "Types/ItemTypes.h"
#include "Types/SaveTypes.h"
#include "ItemManager.generated.h"


class UItemSlot;
class UGemItemDataAsset;
class UEquipmentSlot;
class UEquipmentInstance;
class UWeaponItemDataAsset;
class AWeaponActor;
class UEquipmentItemDataAsset;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UItemManager : public UActorComponent, public IItemContainer
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemManager();
	virtual void InitializeComponent() override;
	// 장비 해제

	void EquipWeapon(UEquipmentInstance* WeaponItemInstance);
	
	virtual EItemContainerType GetItemContainerType() const override;
	virtual UItemInstance* GetItem(int32 Index) override;
	void UnequipAt(int32 Index);
	void EquipTo(int32 Index, UEquipmentInstance* EquipmentInstance);
	void EquipGemTo(int32 Index, UEquipmentInstance* EquipmentInstance);
	virtual bool SetItemAt(UItemInstance* ItemInstance, int32 Index) override;
	virtual bool CanReceiveItem(UItemInstance* ItemInstance, int32 Index) override;
	virtual bool SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex) override;
	
	// 스테이지 저장 복구(직렬화 구조체 사용)
	TArray<FSavedEquipmentData> GetEquipmentSaveData();
	TArray<FSavedEquipmentData> GetGemSaveData();
	void RestoreFromSaveData(const TArray<FSavedEquipmentData>& Data);
	void RestoreGemFromSaveData(const TArray<FSavedEquipmentData>& Data);

	// GameInstance에서 장비 데이터 받아오는 함수
	//void RestoreEquipment(TArray<TObjectPtr<UEquipmentInstance>>  EquipmentData);
	// Getter
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWeaponActor* GetCurrentWeapon() const { return CurrentWeapon; }

	TArray<TObjectPtr<UEquipmentInstance>> GetEquipments();
	//TArray<TObjectPtr<UEquipmentInstance>> GetGemEquipments();
	//void RestoreGemEquipments(const TArray<TObjectPtr<UEquipmentInstance>>& GemData);
	void Clear();

	// 아이템 장착 해제 시 스탯 수정
	void UnequipWeapon();
	TArray<TObjectPtr<UEquipmentSlot>>& GetSkillGemSlots();
	UEquipmentSlot* GetEquipmentSlot(EEquipmentType EquipmentType);
	
	// 장비 Stat 변경시 사용
	UFUNCTION()
	void OnEquipmentStatChanged(EEquipmentType Type, UEquipmentInstance* Instance);

	UPROPERTY(BlueprintAssignable)
	FOnItemSlotChanged OnEquipmentSlotChanged;

	
	
	
private:


	// 장착중인 무기
	UPROPERTY(EditDefaultsOnly, Category = "Item|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeaponActor> CurrentWeapon;
	// 장착 중인 장비 데이터
	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UEquipmentSlot>> EquipmentSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UEquipmentSlot>> GemSlots;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 MaxSKillGemCount = 4;
	// 버프 ID
	TMap<EEquipmentType, TArray<int32>> EquipmentBuffIDs;
};
